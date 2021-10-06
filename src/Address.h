#pragma once
#include "GlobalDefinitions.h"

#define ADDR_BOOT_ROM_START 0x0000
#define ADDR_BOOT_ROM_END 0x00FF

#define ADDR_INTERRUPT_FLAG 0xFF0F
#define ADDR_INTERRUPT_ENABLE 0xFFFF
#define ADDR_SOUND_START 0xFF10
#define ADDR_SOUND_END 0xFF26
#define ADDR_PPU_START 0xFF40
#define ADDR_PPU_END 0xFF4B
#define ADDR_JOYPAD_INPUT 0xFF00
#define ADDR_SERIAL_START 0xFF01
#define ADDR_SERIAL_END 0xFF02
#define ADDR_TIMER_START 0xFF04
#define ADDR_TIMER_END 0xFF07

#define ADDR_PPU_REG_CONTROL 0xFF40
#define ADDR_PPU_REG_STATUS 0xFF41
#define ADDR_PPU_REG_SCROLL_Y 0xFF42
#define ADDR_PPU_REG_SCROLL_X 0xFF43
#define ADDR_PPU_REG_Y_COORD 0xFF44
#define ADDR_PPU_REG_LY_COMPARE 0xFF45
#define ADDR_PPU_REG_DMA_TRANSFER 0xFF46
#define ADDR_PPU_REG_BG_PALETTE_DATA 0xFF47
#define ADDR_PPU_REG_OBJ_PALETTE_0_DATA 0xFF48
#define ADDR_PPU_REG_OBJ_PALETTE_1_DATA 0xFF49
#define ADDR_PPU_REG_OBJ_WINDOW_Y_POS 0xFF4A
#define ADDR_PPU_REG_OBJ_WINDOW_X_POS_MIN_7 0xFF4B

#define ADDR_VIDEO_RAM_START 0x8000
#define ADDR_VIDEO_RAM_END 0x9FFF
#define ADDR_EXTERNAL_RAM_START 0xA000
#define ADDR_EXTERNAL_RAM_END 0xBFFF
#define ADDR_GENERAL_RAM_START 0xC000
#define ADDR_GENERAL_RAM_END 0xDFFF
#define ADDR_OAM_RAM_START 0xFE00
#define ADDR_OAM_RAM_END 0xFE9F
#define ADDR_HIGH_RAM_START 0xFF80
#define ADDR_HIGH_RAM_END 0xFFFF

#define ADDR_VIDEO_RAM_BLOCK_ZERO 0x8000
#define ADDR_VIDEO_RAM_BLOCK_ONE 0x8800
#define ADDR_VIDEO_RAM_BLOCK_TWO 0x9000

class Address
{
private:
    u16 store;
public:
    static const int ADDRESSSPACE = 65536;
    Address(u16 address) : store(address) {}
    Address() : store(0x0) {}

    bool InRange(Address start, Address end);

    operator unsigned short() const
    {
        return store;
    }

    // postfix x++
    Address operator++(int)
    {
        Address copy(*this);
        operator++();
        return copy;
    }

    // prefix ++x
    Address& operator++()
    {
        store++;
        return *this;
    }

    ~Address() {}
};

inline bool Address::InRange(Address start, Address end)
{
    return store >= start.store && store <= end.store;
}