#include "Tile.h"
#include <cstring>
#include <random>

void Tile::LoadData(Byte data[16])
{
    memcpy(this->data, data, 16);
}

// 8 x 8
Byte Tile::GetPixel(int x, int y)
{
    int pixelNum = (y * 8) + x;
    Byte upperIdx = (pixelNum / 8) * 2;
    Byte lowerIdx = upperIdx + 1;
    Byte upper = this->data[upperIdx];
    Byte lower = this->data[lowerIdx];
    Byte bitIndex = 1 << (7 - x);

    return ((upper & bitIndex) ? 1 : 0) + ((lower & bitIndex) ? 2 : 0);
}