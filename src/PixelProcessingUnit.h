#pragma once
#include "IMemoryMappable.h"
#include "Gpu.h"
#include "Memory.h"

#define LCD_REG_CONTROL mem[0]
#define LCD_REG_STATUS mem[1]
#define LCD_REG_SCROLL_Y mem[2]
#define LCD_REG_SCROLL_X mem[3]
#define LCD_REG_Y_COORD mem[4]
#define LCD_REG_LY_COMPARE mem[5]
#define LCD_REG_DMA_TRANSFER mem[6]
#define LCD_REG_BG_PALETTE_DATA mem[7]
#define LCD_REG_OBJ_PALETTE_0_DATA mem[8]
#define LCD_REG_OBJ_PALETTE_1_DATA mem[9]
#define LCD_REG_OBJ_WINDOW_Y_POS mem[10]
#define LCD_REG_OBJ_WINDOW_X_POS_MIN_7 mem[11]

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
    Byte mem[12];

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