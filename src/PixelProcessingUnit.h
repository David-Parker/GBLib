#pragma once
#include "BGMap.h"
#include "IMemoryMappable.h"
#include "Memory.h"
#include "GlobalDefinitions.h"
#include "GraphicsManager.h"
#include "Tile.h"
#include "RegisterU8.h"

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

enum LCD_STAT_FLAGS
{
    MATCH_INT_SOURCE = 64,
    OAM_INT_SOURCE = 32,
    VBLANK_INT_SOURCE = 16,
    HBLANK_INT_SOURCE = 8,
    MATCH_FLAG = 4,
    MODE_FLAG_UPPER = 2,
    MODE_FLAG_LOWER = 1
};

enum LCD_MODE
{
    HBLANK,
    VBLANK,
    OBJ_SEARCH,
    VIDEO_READ
};

class PixelProcessingUnit : public IMemoryMappable
{
private:
    RegisterU8 LCDC;
    RegisterU8 STAT;
    RegisterU8 SCY;
    RegisterU8 SCX;
    RegisterU8 LY;
    RegisterU8 LYC;
    RegisterU8 DMA;
    RegisterU8 BGP;
    RegisterU8 OBP0;
    RegisterU8 OBP1;
    RegisterU8 WY;
    RegisterU8 WX;

    Memory* pMemory;
    Byte mem[(ADDR_PPU_END - ADDR_PPU_START) + 1];
    BGMap backgroundMap;
    GraphicsManager gManager;
    bool lcdInit;

    LCD_MODE mode;
    u64 lastUpdateClock;

    LCD_CTRL_FLAGS lcd_flags;

    Address GetBGCodeArea();
    Address GetBGCharArea();
    Address GetWindowCodeArea();

    void TurnOnLCD();
    void BufferScanLine();
    void Draw();
    void LoadColorPalette();
    bool LCDIsOn();
    void TestLYCMatch();

public:
    PixelProcessingUnit(Memory* pMemory);
    ~PixelProcessingUnit();

    void Write(Address address, Byte value);
    Byte Read(Address address);
    void Tick(u64 cycles);
};