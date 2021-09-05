#include "Tile.h"

Tile::Tile(Byte data[16])
{
    Byte bitIndex;
    int line = 0;

    for (int i = 0; i < 16; i++)
    {
        Byte upper = data[i++];
        Byte lower = data[i];

        for (int j = 0; j < 8; j++)
        {
            bitIndex = 1 << (7 - j);

            pixels[line][j] = ((upper & bitIndex) ? 1 : 0) + ((lower & bitIndex) ? 2 : 0);
        }

        line++;
    }
}
