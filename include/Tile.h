#pragma once
#include "GlobalDefinitions.h"
#include "Memory.h"

class Tile
{
public: 
    static Byte GetPixel(Memory* pMemory, Address data, int x, int y);
};