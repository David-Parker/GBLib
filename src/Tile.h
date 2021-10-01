#pragma once
#include "GlobalDefinitions.h"

class Tile
{
private:
    Byte data[16];

public: 
    Tile() {};
    ~Tile() {}

    void LoadData(Byte data[16]);
    Byte GetPixel(int x, int y);
};

// 8 x 8
inline Byte Tile::GetPixel(int x, int y)
{
    int pixelNum = (y * 8) + x;
    Byte upperIdx = (pixelNum / 8) * 2;
    Byte lowerIdx = upperIdx + 1;
    Byte upper = this->data[upperIdx];
    Byte lower = this->data[lowerIdx];
    Byte bitIndex = 1 << (7 - x);

    return ((upper & bitIndex) ? 1 : 0) + ((lower & bitIndex) ? 2 : 0);
}