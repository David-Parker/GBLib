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