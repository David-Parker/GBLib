#pragma once

#include "Memory.h"
#include "GlobalDefinitions.h"
#include "GraphicsManager.h"
#include "Tile.h"

// Background map
class BGMap
{
private:
    Memory* pMemory;

public:
    BGMap(Memory* pMemory);
    ~BGMap();

    Byte GetPixel(Address tileData, Address tileMap, int x, int y);
    Byte GetTile(Address tileMap, int xTile, int yTile);
};