#include "MBC.h"

void MBC1::LoadFromFile(std::string& path)
{
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

        this->currentROMBank = (this->currentROMBank & 0b01100000) + value;

    }
    else if (AddressInRange(address, ADDR_MBC_MODE_SELECT_START, ADDR_MBC_MODE_SELECT_END))
    {
        switch (value)
        {
        case 0x00:
            this->currentMode = 0;
            break;
        case 0x01:
            this->currentMode = 1;
            break;
        default:
            throw std::exception("Unknown MBC1 Mode.");
        }
    }
    else if (AddressInRange(address, ADDR_MBC_RAM_BANK_START, ADDR_MBC_RAM_BANK_END))
    {
        // ROM mode
        if (this->currentMode == 0)
        {
            value = value & 0b00000011;

            value = value << 5;

            this->currentROMBank = (this->currentROMBank & value) + (this->currentROMBank & 0b00011111);
        }
        else // RAM mode
        {
            this->currentRAMBank = value;
        }
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
        return this->romBanks[this->currentROMBank]->Read(address);
    }
    else if (AddressInRange(address, ADDR_EXTERNAL_RAM_START, ADDR_EXTERNAL_RAM_END))
    {
        if (this->currentMode == 0)
        {
            return this->ramBanks[0]->Read(address);
        }
        else
        {
            return this->ramBanks[this->currentRAMBank]->Read(address);
        }
    }
    else
    {
        throw std::exception("Invalid MBC memory read.");
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