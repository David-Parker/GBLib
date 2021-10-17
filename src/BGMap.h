#pragma once

#include "Memory.h"
#include "GlobalDefinitions.h"
#include "GraphicsManager.h"
#include "Tile.h"
#include "RegisterU8.h"

// Background map
class BGMap
{
private:
    Memory* pMemory;

public:
    Byte palette[4];

    BGMap::BGMap(Memory* pMemory)
        : pMemory(pMemory)
    {
        for (int i = 0; i < 4; ++i)
        {
            palette[i] = i;
        }
    }

    ~BGMap() {}

    Byte GetPixel(Address tileData, Address tileMap, int x, int y);
    Byte GetTile(Address tileMap, int xTile, int yTile);
    void LoadColorPalette(RegisterU8& BGP);
};