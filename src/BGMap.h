#pragma once

#include "Memory.h"
#include "GlobalDefinitions.h"
#include "GraphicsManager.h"
#include "Tile.h"

// Background map
class BGMap
{
private:
    Tile tilePatternTable[256];
    Address tileMapStart;
    Byte tileMap[TILEMAP_SIZE][TILEMAP_SIZE];
    Memory* pMemory;

public:
    BGMap(Memory* pMemory);
    ~BGMap();

    void LoadTilePatternTable(Address start);
    void LoadTileMap(Address start);
    Byte GetPixel(int x, int y);
    Byte GetTile(int xTile, int yTile);
};

inline Byte BGMap::GetPixel(int x, int y)
{
    int tileX = x / 8;
    int tileY = y / 8;
    int pixelX = x % 8;
    int pixelY = y % 8;

    Byte tileIndex = this->GetTile(tileX, tileY);
    Tile& tile = this->tilePatternTable[tileIndex];

    return tile.GetPixel(pixelX, pixelY);
}

inline Byte BGMap::GetTile(int xTile, int yTile)
{
    int offset = (yTile * 32) + xTile;
    return this->pMemory->Read(this->tileMapStart + offset);
}