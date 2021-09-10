#pragma once
#include "GlobalDefinitions.h"

class Tile
{
public:
    Byte pixels[8][8] = {};
    Tile() : pixels() {};
    Tile(Byte data[16]);
    ~Tile() {}
};