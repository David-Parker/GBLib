#pragma once
#include "GlobalDefinitions.h"
#include "Memory.h"

class Tile
{
private:
    Address data;
    Memory* pMemory;

public: 
    Tile() {};
    ~Tile() {}

    void LoadTileData(Memory* pMemory, Address data);
    Byte GetPixel(int x, int y);
};

// 8 x 8
inline Byte Tile::GetPixel(int x, int y)
{
    int pixelNum = (y * 8) + x;
    Byte upperIdx = (pixelNum / 8) * 2;
    Byte lowerIdx = upperIdx + 1;
    Byte upper = this->pMemory->Read(data + upperIdx);
    Byte lower = this->pMemory->Read(data + lowerIdx);
    Byte bitIndex = 1 << (7 - x);

    return ((upper & bitIndex) ? 1 : 0) + ((lower & bitIndex) ? 2 : 0);
}