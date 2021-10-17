#include "Tile.h"

Byte Tile::GetPixel(Memory* pMemory, Address data, int x, int y)
{
    int pixelNum = (y * 8) + x;
    Byte upperIdx = (pixelNum / 8) * 2;
    Byte lowerIdx = upperIdx + 1;
    Byte upper = pMemory->Read(data + upperIdx);
    Byte lower = pMemory->Read(data + lowerIdx);
    Byte bitIndex = 1 << (7 - x);

    return ((upper & bitIndex) ? 1 : 0) + ((lower & bitIndex) ? 2 : 0);
}