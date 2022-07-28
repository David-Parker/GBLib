#pragma once

#include "GlobalDefinitions.h"

struct Sprite
{
    Byte yPos;
    Byte xPos;
    Byte tileIndex;
    Byte attr;
    Byte height;
    Byte index;

    bool operator >(const Sprite& other) const
    {
        return xPos > other.xPos;
    }
};