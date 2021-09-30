#pragma once
#include "IMemoryMappable.h"
#include "Memory.h"
#include "GlobalDefinitions.h"
#include "GraphicsManager.h"
#include "Tile.h"

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
    Memory* pMemory;
    Byte mem[ADDR_PPU_END - ADDR_PPU_START + 1];

    const static u16 sizeX = 32;
    const static u16 sizeY = 32;
    Tile tilePatternTable[256];
    Byte tileMap[sizeY][sizeX];
    GraphicsManager gManager;

    LCD_CTRL_FLAGS lcd_flags;

    Address GetBGCodeArea();
    Address GetBGCharArea();
    Address GetWindowCodeArea();

    void TurnOnLCD();
    void LoadTilePatternTable(Address start);
    void LoadTileMap(Address start);
    void LoadColorPalette();
    void Draw();

public:
    PixelProcessingUnit(Memory* pMemory);
    ~PixelProcessingUnit();

    void Write(Address address, Byte value);
    Byte Read(Address address);
};