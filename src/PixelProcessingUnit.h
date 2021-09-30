#pragma once
#include "IMemoryMappable.h"
#include "Gpu.h"
#include "Memory.h"

enum LCD_CTRL_FLAGS
{
    LCD_ON = 128,
    WINDOW_CODE_AREA_SELECT = 64,
    WINDOWING_ON = 32,
    BG_CHAR_DATA_SELECT = 16,
    BG_CODE_AREA_SELECT = 8,
    OBJ_BLOCK_SELECT = 4,
    OBJ_ON = 2,
    BG_DISPLAY_ON = 1,
};

class PixelProcessingUnit : public IMemoryMappable
{
private:
    Gpu gpu;
    Memory* pMemory;
    Byte mem[ADDR_PPU_END - ADDR_PPU_START + 1];

    LCD_CTRL_FLAGS lcd_flags;

    Address GetBGCodeArea();
    Address GetBGCharArea();
    Address GetWindowCodeArea();

public:
    PixelProcessingUnit(Memory* pMemory);
    ~PixelProcessingUnit();

    void Write(Address address, Byte value);
    Byte Read(Address address);
};