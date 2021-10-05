#include "BGMap.h"

BGMap::BGMap(Memory* pMemory) 
    : pMemory(pMemory)
{
}

BGMap::~BGMap()
{
}

void BGMap::LoadTilePatternTable(Address start)
{
    if (start == 0x8800)
    {
        throw std::exception("Secondary tile map is not implemented.");
    }

    for (int i = 0; i < 256; i++)
    {
        Tile& tile = this->tilePatternTable[i];
        tile.LoadTileData(this->pMemory, start);
        start = start + 16;
    }
}

void BGMap::LoadTileMap(Address start)
{
    this->tileMapStart = start;
}