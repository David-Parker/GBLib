#include "MBC.h"

void MBC1::LoadFromFile(std::string& path)
{
    if (this->header.ramSize > 0x03)
    {
        throw std::exception("MBC1 does not support more than 32kb RAM.");
    }

    this->ramSize = (MBC1_RAM_SIZES)this->header.ramSize;

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

void MBC1::Write(Address address, Byte value)
{
    if (AddressInRange(address, ADDR_MBC_RAM_ENABLE_START, ADDR_MBC_RAM_ENABLE_END))
    {
        if (this->ramSize == MBC1_RAM_SIZES::NO_RAM)
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

        // MBC always sets lower 5 bits of ROM bank select register to 0x01 when 0x00 is written.
        // This means if bits 5-6 are enabled for 0x20, 0x40, or 0x60, you will instead index
        // banks 0x21, 0x41, and 0x61.
        if (value == 0)
        {
            value = 1;
        }

        this->romBankRegister = value;

    }
    else if (AddressInRange(address, ADDR_MBC_MODE_SELECT_START, ADDR_MBC_MODE_SELECT_END))
    {
        switch (value)
        {
        case 0x00:
            this->currentMode = MBC1_BANK_MODE::ROM_MODE;
            break;
        case 0x01:
            this->currentMode = MBC1_BANK_MODE::RAM_MODE;
            break;
        default:
            throw std::exception("Unknown MBC1 Mode.");
        }
    }
    else if (AddressInRange(address, ADDR_MBC_RAM_BANK_START, ADDR_MBC_RAM_BANK_END))
    {
        value = value & 0b00000011;

        this->extraBankRegister = value;
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
            return 0xFF;
        }

        return this->ramBanks[GetCurrentRAMBank()]->Read(address);
    }
    else
    {
        throw std::exception("Invalid MBC memory read.");
    }
}

u8 MBC1::GetCurrentROMBank()
{
    if (this->currentMode == MBC1_BANK_MODE::ROM_MODE)
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
    if (this->currentMode == MBC1_BANK_MODE::ROM_MODE)
    {
        return 0;
    }
    else
    {
        return this->extraBankRegister;
    }
}

void MBC3::LoadFromFile(std::string& path)
{

}

void MBC3::Write(Address address, Byte value)
{

}

Byte MBC3::Read(Address address)
{
    return 0xFF;
}