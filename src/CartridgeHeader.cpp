#include "CartridgeHeader.h"
#include <iostream>

CartridgeHeader::CartridgeHeader()
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
    std::cout << "ROM Size: " << (32 << romSize) << " kb" << std::endl;
    std::cout << "RAM Size: " << (32 << ramSize) << " kb" << std::endl;
    std::cout << std::endl;
}

void CartridgeHeader::Read(std::string& path)
{
    Byte buf[0x150];

    FILE *filepoint;
    errno_t err;

    if ((err = fopen_s(&filepoint, path.c_str(), "rb")) != 0)
    {
        throw std::exception("Could not open ROM file.");
    }
    else
    {
        size_t bytesRead = fread(buf, sizeof(Byte), 0x150, filepoint);

        if (bytesRead == 0)
        {
            throw std::exception("Failed to read ROM file.");
        }

        fclose(filepoint);
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
    isCGB = cgb_flag & 0x80;

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