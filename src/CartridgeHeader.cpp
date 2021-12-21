#include "CartridgeHeader.h"
#include <iostream>

CartridgeHeader::CartridgeHeader()
    : isDMG(true), isCGB(false), isSGB(false)
{
    for (int i = 0; i < 256; ++i)
    {
        cartridge_types[i] = "Unknown";
    }

    cartridge_types[0x00] = "ROM ONLY";
    cartridge_types[0x01] = "MBC1";
    cartridge_types[0x02] = "MBC1+RAM";
    cartridge_types[0x03] = "MBC1+RAM+BATTERY";
    cartridge_types[0x05] = "MBC2";
    cartridge_types[0x06] = "MBC2+BATTERY";
    cartridge_types[0x08] = "ROM+RAM";
    cartridge_types[0x09] = "ROM+RAM+BATTERY";
    cartridge_types[0x0B] = "MMM01";
    cartridge_types[0x0C] = "MMM01+RAM";
    cartridge_types[0x0D] = "MMM01+RAM+BATTERY";
    cartridge_types[0x0F] = "MBC3+TIMER+BATTERY";
    cartridge_types[0x10] = "MBC3+TIMER+RAM+BATTERY";
    cartridge_types[0x11] = "MBC3";
    cartridge_types[0x12] = "MBC3+RAM";
    cartridge_types[0x13] = "MBC3+RAM+BATTERY";
    cartridge_types[0x19] = "MBC5";
    cartridge_types[0x1A] = "MBC5+RAM";
    cartridge_types[0x1B] = "MBC5+RAM+BATTERY";
    cartridge_types[0x1C] = "MBC5+RUMBLE";
    cartridge_types[0x1D] = "MBC5+RUMBLE+RAM";
    cartridge_types[0x1E] = "MBC5+RUMBLE+RAM+BATTERY";
    cartridge_types[0x20] = "MBC6";
    cartridge_types[0x22] = "MBC7+SENSOR+RUMBLE+RAM+BATTERY";
    cartridge_types[0xFC] = "POCKET CAMERA";
    cartridge_types[0xFD] = "BANDAI TAMA5";
    cartridge_types[0xFE] = "HuC3";
    cartridge_types[0xFF] = "HuC1+RAM+BATTERY";
}

CartridgeHeader::~CartridgeHeader()
{
}

void CartridgeHeader::PrintInfo()
{
    std::cout << "Title: " << title << std::endl;
    std::cout << "GameBoy Color: " << isCGB << std::endl;
    std::cout << "Super GameBoy: " << isSGB << std::endl;
    std::cout << "Cartridge Type: " << cartridge_types[cartridgeType] << std::endl;
    std::cout << "ROM Size: " << NumROMBanks() * 16 << " kb" << std::endl;
    std::cout << "RAM Size: " << NumRAMBanks() * 8 << " kb" << std::endl;
    std::cout << std::endl;
}

void CartridgeHeader::Read(std::string path)
{
    Byte buf[0x150];

    std::ifstream file(path, std::ios::binary);

    if (!file)
    {
        throw std::runtime_error("Could not open ROM file.");
    }
    else
    {
        file.read(reinterpret_cast<char*>(&buf[0]), 0x150);
    }

    // Get the title
    char sZtitle[16] = {};
    int loc = 0;

    for (int i = 0x134; i <= 0x142; ++i)
    {
        sZtitle[loc++] = buf[i];
    }

    title = sZtitle;

    // Check if game is Game Boy Color
    Byte cgb_flag = buf[0x143];

    if (cgb_flag == 0x80)
    {
        isCGB = true;
    }
    else if (cgb_flag == 0xC0)
    {
        isCGB = true;
        isDMG = false;
    }

    // Used to force CGB emulation
    // isCGB = true;

    // Check if game is Super Game Boy
    Byte sgb_flag = buf[0x146];
    isSGB = sgb_flag == 0x03;

    // Get cartridge type
    cartridgeType = buf[0x147];

    // ROM size
    romSize = buf[0x148];

    // RAM size
    ramSize = buf[0x149];
}

int CartridgeHeader::NumROMBanks()
{
    switch (this->romSize)
    {
    case 0x0:
        return 2;
        break;
    case 0x1:
        return 4;
        break;
    case 0x2:
        return 8;
        break;
    case 0x3:
        return 16;
        break;
    case 0x4:
        return 32;
        break;
    case 0x5:
        return 64;
        break;
    case 0x6:
        return 128;
        break;
    case 0x7:
        return 256;
        break;
    case 0x8:
        return 512;
        break;
    case 0x52:
        return 72;
        break;
    case 0x53:
        return 80;
        break;
    case 0x54:
        return 96;
        break;
    default:
        throw std::runtime_error("Invalid ROM size.");
    }
}

int CartridgeHeader::NumRAMBanks()
{
    switch (this->ramSize)
    {
    case 0x0:
        return 0;
        break;
    case 0x1:
        return 0;
        break;
    case 0x2:
        return 1;
        break;
    case 0x3:
        return 4;
        break;
    case 0x4:
        return 16;
        break;
    case 0x5:
        return 8;
        break;
    default:
        throw std::runtime_error("Invalid RAM size.");
    }
}

bool CartridgeHeader::HasBattery()
{
    switch (this->cartridgeType)
    {
        case CART_MBC1_RAM_BATTERY:
        case CART_MBC2_BATTERY:
        case CART_ROM_RAM_BATTERY:
        case CART_MMM01_RAM_BATTERY:
        case CART_MBC3_TIMER_BATTERY:
        case CART_MBC3_TIMER_RAM_BATTERY:
        case CART_MBC3_RAM_BATTERY:
        case CART_MBC5_RAM_BATTERY:
        case CART_MBC5_RUMBLE_RAM_BATTERY:
        case CART_MBC7_SENSOR_RUMBLE_RAM_BATTERY:
        case CART_HUC1_RAM_BATTERY:
            return true;
        default:
            return false;
    }
}