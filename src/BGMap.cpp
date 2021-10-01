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
        throw new std::exception("Secondary tile map is not implemented.");
    }

    Byte buf[16];

    for (int i = 0; i < 256; i++)
    {
        // Read in the next tile (16 bytes)
        for (int j = 0; j < 16; j++)
        {
            buf[j] = pMemory->Read(start++);
        }

        Tile& tile = this->tilePatternTable[i];
        tile.LoadData(buf);
    }
}

void BGMap::LoadTileMap(Address start)
{
    int x = 0;

    for (int i = 0; i < TILEMAP_SIZE; ++i)
    {
        for (int j = 0; j < TILEMAP_SIZE; j++)
        {
            tileMap[i][j] = pMemory->Read(start++);
        }
    }
}