#include "MBC.h"
#include<iostream>
#include<fstream>

void MBC::LoadROMFromFile(std::string path)
{
    if (this->header.ramSize > 0x03)
    {
        throw std::runtime_error("MBC does not support more than 32kb RAM.");
    }

    this->ramSize = (MBC_RAM_SIZES)this->header.ramSize;

    for (int i = 0; i < this->header.NumROMBanks(); ++i)
    {
        ROM* bank;

        if (i == 0)
        {
            bank = new ROM(ADDR_ROM_BANK_0_START, ADDR_ROM_BANK_0_END);
            bank->LoadFromFile(path, 0, ROM_BANK_BYTES);
        }
        else
        {
            bank = new ROM(ADDR_ROM_BANK_EXTENDABLE_START, ADDR_ROM_BANK_EXTENDABLE_END);
            bank->LoadFromFile(path, ROM_BANK_BYTES * i, ROM_BANK_BYTES);
        }

        this->romBanks.push_back(bank);
    }

    for (int i = 0; i < this->header.NumRAMBanks(); ++i)
    {
        RAM* bank;

        bank = new RAM(ADDR_EXTERNAL_RAM_START, ADDR_EXTERNAL_RAM_END);

        this->ramBanks.push_back(bank);
    }
}

void MBC::SaveToFile(std::string path)
{
    if (this->ramSize == MBC_RAM_SIZES::NO_RAM || this->ramBanks.size() == 0)
    {
        return;
    }

    std::ofstream file(path, std::fstream::out | std::ios::binary | std::ios::trunc);

    if (!file)
    {
        throw std::runtime_error("Could not open save file. Check if the passed directory exists.");
    }

    // Write each bank
    for (auto& ram : this->ramBanks)
    {
        Byte buf[RAM_BANK_BYTES] = { 0 };

        for (int i = ram->GetStartAddress(); i <= ram->GetEndAddress(); ++i)
        {
            buf[i - ram->GetStartAddress()] = ram->Read(i);
        }

        file.write(reinterpret_cast<char*>(&buf[0]), RAM_BANK_BYTES);
    }
}

void MBC::LoadRAMFromSave(std::string path)
{
    if (this->ramBanks.size() == 0)
    {
        return;
    }

    std::ifstream file(path, std::ios::binary);

    // No save file exists
    if (!file)
    {
        return;
    }

    // Read each bank
    for (auto& ram : this->ramBanks)
    {
        Byte buf[RAM_BANK_BYTES];
        file.read(reinterpret_cast<char*>(&buf[0]), RAM_BANK_BYTES);

        for (int i = ram->GetStartAddress(); i <= ram->GetEndAddress(); ++i)
        {
            ram->Write(i, buf[i - ram->GetStartAddress()]);
        }
    }
}

void MBC1::Write(Address address, Byte value)
{
    // ROM
    if (AddressInRange(address, ADDR_MBC_RAM_ENABLE_START, ADDR_MBC_RAM_ENABLE_END))
    {
        if (this->ramSize == MBC_RAM_SIZES::NO_RAM)
        {
            return;
        }

        u8 mask = 0b00001111;
        value = value & mask;

        if (value == 0x0A)
        {
            this->ramEnabled = true;
        }
        else
        {
            this->ramEnabled = false;
        }
    }
    else if (AddressInRange(address, ADDR_MBC_ROM_BANK_START, ADDR_MBC_ROM_BANK_END))
    {
        value = value & 0b00011111;

        // MBC1 always sets lower 5 bits of ROM bank select register to 0x01 when 0x00 is written.
        // This means if bits 5-6 are enabled for 0x20, 0x40, or 0x60, you will instead index
        // banks 0x21, 0x41, and 0x61.
        if (value == 0)
        {
            value = 1;
        }

        this->romBankRegister = value;

    }
    else if (AddressInRange(address, ADDR_MBC_RAM_BANK_START, ADDR_MBC_RAM_BANK_END))
    {
        value = value & 0b00000011;

        this->extraBankRegister = value;
    }
    else if (AddressInRange(address, ADDR_MBC_MODE_SELECT_START, ADDR_MBC_MODE_SELECT_END))
    {
        switch (value)
        {
        case 0x00:
            this->currentMode = MBC_BANK_MODE::ROM_MODE;
            break;
        case 0x01:
            this->currentMode = MBC_BANK_MODE::RAM_MODE;
            break;
        default:
            throw std::runtime_error("Unknown MBC1 Mode.");
        }
    }

    // RAM
    else if (AddressInRange(address, ADDR_EXTERNAL_RAM_START, ADDR_EXTERNAL_RAM_END))
    {
        if (!this->ramEnabled)
        {
            return;
        }

        this->ramBanks[GetCurrentRAMBank()]->Write(address, value);
    }
}

Byte MBC1::Read(Address address)
{
    // ROM
    if (AddressInRange(address, ADDR_ROM_BANK_0_START, ADDR_ROM_BANK_0_END))
    {
        return this->romBanks[0]->Read(address);
    }
    else if (AddressInRange(address, ADDR_ROM_BANK_EXTENDABLE_START, ADDR_ROM_BANK_EXTENDABLE_END))
    {
        return this->romBanks[GetCurrentROMBank()]->Read(address);
    }
    // RAM
    else if (AddressInRange(address, ADDR_EXTERNAL_RAM_START, ADDR_EXTERNAL_RAM_END))
    {
        if (!this->ramEnabled)
        {
            return INVALID_READ;
        }

        return this->ramBanks[GetCurrentRAMBank()]->Read(address);
    }
    else
    {
        throw std::runtime_error("Invalid MBC memory read.");
    }
}

u8 MBC1::GetCurrentROMBank()
{
    if (this->currentMode == MBC_BANK_MODE::ROM_MODE)
    {
        return (this->romBankRegister) + (this->extraBankRegister << 5);
    }
    else
    {
        return this->romBankRegister;
    }
}

u8 MBC1::GetCurrentRAMBank()
{
    if (this->currentMode == MBC_BANK_MODE::ROM_MODE)
    {
        return 0;
    }
    else
    {
        return this->extraBankRegister;
    }
}

void MBC2::Write(Address address, Byte value)
{
    // RAM enable / ROM bank number
    if (AddressInRange(address, ADDR_MBC2_RAM_ENABLE_START, ADDR_MBC2_RAM_ENABLE_END))
    {
        if (address & 0x100)
        {
            // Write to ROM bank number, if 0 is written, bank 1 is used instead.
            if (value == 0)
            {
                value = 1;
            }

            this->romBankRegister = value & 0xF;
        }
        else
        {
            // Write to RAM enable register
            if (value == 0xA)
            {
                this->ramEnabled = true;
            }
            else
            {
                this->ramEnabled = false;
            }
        }
    }
}

Byte MBC2::Read(Address address)
{
    // ROM
    if (AddressInRange(address, ADDR_ROM_BANK_0_START, ADDR_ROM_BANK_0_END))
    {
        return this->romBanks[0]->Read(address);
    }
    else if (AddressInRange(address, ADDR_ROM_BANK_EXTENDABLE_START, ADDR_ROM_BANK_EXTENDABLE_END))
    {
        return this->romBanks[this->romBankRegister]->Read(address);
    }

    // Built in RAM
    if (AddressInRange(address, ADDR_EXTERNAL_RAM_START, ADDR_EXTERNAL_RAM_END))
    {
        if (!this->ramEnabled)
        {
            return INVALID_READ;
        }

        // Only the bottom 9 bits of the address are used to index the address range 0xA000 to 0xA1FF
        Address bits = address & 0x1FF;
        Address finalAddress = ADDR_EXTERNAL_RAM_START + bits;

        // Only lower 4 bits are returned
        return this->ramBanks[0]->Read(finalAddress) & 0xF;
    }

    return INVALID_READ;
}

void MBC3::Write(Address address, Byte value)
{
    // ROM
    if (AddressInRange(address, ADDR_MBC_RAM_ENABLE_START, ADDR_MBC_RAM_ENABLE_END))
    {
        if (this->ramSize == MBC_RAM_SIZES::NO_RAM)
        {
            return;
        }

        u8 mask = 0b00001111;
        value = value & mask;

        if (value == 0x0A)
        {
            this->ramEnabled = true;
        }
        else
        {
            this->ramEnabled = false;
        }
    }
    else if (AddressInRange(address, ADDR_MBC_ROM_BANK_START, ADDR_MBC_ROM_BANK_END))
    {
        value = value & 0b01111111;

        this->romBankRegister = value;

    }
    else if (AddressInRange(address, ADDR_MBC_RAM_BANK_START, ADDR_MBC_RAM_BANK_END))
    {
        if (value >= 0x00 && value <= 0x03)
        {
            this->ramBankRegister = value;
            this->currentMode = MBC3_REGISTER_MODE::RAM_REG_MODE;
        }
        else if (value >= 0x08 && value <= 0x0C)
        {
            this->currentMode = MBC3_REGISTER_MODE::RTC_REG_MODE;
            this->currentRtcRegister = value - 0x08;
        }
    }
    else if (AddressInRange(address, ADDR_MBC3_LATCH_CLOCK_START, ADDR_MBC3_LATCH_CLOCK_END))
    {
        if (this->latchRegister == 0x00 && value == 0x01)
        {
            // Latch the current time into the rtc register
            // TODO: This is not implemented yet
            this->latchRegister = 0xFF;
        }
        else
        {
            this->latchRegister = value;
        }
    }
    // RAM
    else if (AddressInRange(address, ADDR_EXTERNAL_RAM_START, ADDR_EXTERNAL_RAM_END))
    {
        if (!this->ramEnabled)
        {
            return;
        }

        if (this->currentMode == MBC3_REGISTER_MODE::RAM_REG_MODE)
        {
            this->ramBanks[GetCurrentRAMBank()]->Write(address, value);
        }
        else
        {
            this->rtcRegisters[this->currentRtcRegister] = value;
        }
    }
}

Byte MBC3::Read(Address address)
{
    // ROM
    if (AddressInRange(address, ADDR_ROM_BANK_0_START, ADDR_ROM_BANK_0_END))
    {
        return this->romBanks[0]->Read(address);
    }
    else if (AddressInRange(address, ADDR_ROM_BANK_EXTENDABLE_START, ADDR_ROM_BANK_EXTENDABLE_END))
    {
        return this->romBanks[GetCurrentROMBank()]->Read(address);
    }
    // RAM
    else if (AddressInRange(address, ADDR_EXTERNAL_RAM_START, ADDR_EXTERNAL_RAM_END))
    {
        if (!this->ramEnabled)
        {
            return INVALID_READ;
        }

        if (this->currentMode == MBC3_REGISTER_MODE::RAM_REG_MODE)
        {
            return this->ramBanks[GetCurrentRAMBank()]->Read(address);
        }
        else
        {
            return this->rtcRegisters[this->currentRtcRegister];
        }
    }
    else
    {
        throw std::runtime_error("Invalid MBC memory read.");
    }
}

u8 MBC3::GetCurrentROMBank()
{
    return this->romBankRegister;
}

u8 MBC3::GetCurrentRAMBank()
{
    return this->ramBankRegister;
}