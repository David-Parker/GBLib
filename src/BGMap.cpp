#include "BGMap.h"

BGMap::BGMap(Memory* pMemory)
    : pMemory(pMemory)
{
}

BGMap::~BGMap()
{
}

Byte BGMap::GetPixel(Address tileData, Address tileMap, int x, int y)
{
    int tileX = x / 8;
    int tileY = y / 8;
    int pixelX = x % 8;
    int pixelY = y % 8;

    Byte tileIndex = this->GetTile(tileMap, tileX, tileY);
    Address tileAddress;

    if (tileData == ADDR_VIDEO_RAM_BLOCK_ZERO)
    {
        tileAddress = ADDR_VIDEO_RAM_BLOCK_ZERO + (tileIndex * 16);
    }
    else if (tileData == ADDR_VIDEO_RAM_BLOCK_ONE)
    {
        s8 tileIndexSigned = (s8)tileIndex;
        tileAddress = ADDR_VIDEO_RAM_BLOCK_TWO + (tileIndexSigned * 16);
    }
    else
    {
        throw std::exception("Cannot read tile data from address.");
    }

    Tile tile(pMemory, tileAddress);
    return tile.GetPixel(pixelX, pixelY);
}

Byte BGMap::GetTile(Address tileMap, int xTile, int yTile)
{
    int offset = (yTile * 32) + xTile;
    return this->pMemory->Read(tileMap + offset);
}