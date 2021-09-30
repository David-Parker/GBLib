#pragma once
#include <vector>
#include "GlobalDefinitions.h"
#include "GraphicsManager.h"
#include "Memory.h"
#include "Tile.h"

class Gpu
{
private:
    Memory* pMemory;
    const static u16 sizeX = 32;
    const static u16 sizeY = 32;
    Tile tilePatternTable[256];
    Byte tileMap[sizeY][sizeX];
    GraphicsManager gManager;

public:
    Gpu(Memory* pMemory) : pMemory(pMemory)
    {}

    ~Gpu() {}

    void TurnOnLCD();
    void LoadTilePatternTable(Address start);
    void LoadTileMap(Address start);
    void LoadColorPalette();
    void Draw();
};