#pragma once
#include "GlobalDefinitions.h"
#include "Memory.h"
#include <string>

#define CART_ROM_ONLY 0x00
#define CART_MBC1 0x01
#define CART_MBC1_RAM 0x02
#define CART_MBC1_RAM_BATTERY 0x03
#define CART_MBC2 0x05
#define CART_MBC2_BATTERY 0x06
#define CART_ROM_RAM 0x08
#define CART_ROM_RAM_BATTERY 0x09
#define CART_MMM01 0x0B
#define CART_MMM01_RAM 0x0C
#define CART_MMM01_RAM_BATTERY 0x0D
#define CART_MBC3_TIMER_BATTERY 0x0F
#define CART_MBC3_TIMER_RAM_BATTERY 0x10
#define CART_MBC3 0x11
#define CART_MBC3_RAM 0x12
#define CART_MBC3_RAM_BATTERY 0x13
#define CART_MBC5 0x19
#define CART_MBC5_RAM 0x1A
#define CART_MBC5_RAM_BATTERY 0x1B
#define CART_MBC5_RUMBLE 0x1C
#define CART_MBC5_RUMBLE_RAM 0x1D
#define CART_MBC5_RUMBLE_RAM_BATTERY 0x1E
#define CART_MBC6 0x20
#define CART_MBC7_SENSOR_RUMBLE_RAM_BATTERY 0x22
#define CART_POCKET_CAMERA 0xFC
#define CART_BANDAI_TAMA5 0xFD
#define CART_HUC3 0xFE
#define CART_HUC1_RAM_BATTERY 0xFF

class CartridgeHeader
{
public:
    std::string title;
    bool isCGB; // GameBoy Color
    bool isSGB; // Super GameBoy
    u8 cartridgeType;
    u8 romSize;
    u8 ramSize;

    char* cartridge_types[256];

    CartridgeHeader();
    ~CartridgeHeader();
    void Read(std::string& path);
    void PrintInfo();
    int NumROMBanks();
    int NumRAMBanks();
    bool HasBattery();
};