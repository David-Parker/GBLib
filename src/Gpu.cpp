#include "Gpu.h"

void Gpu::LoadTileMap()
{
    std::vector<Tile> tiles;
    
    Address start = 0x3000;
    Address end = start + 0x4000;

    for (Address i = start; i < end; i++)
    {
        Byte bytes[16] = {};

        for (int j = 0; j < 16; j++)
        {
            bytes[j] = pMemory->Read(i);

            if (j < 15) i++;
        }

        Tile tile = Tile(bytes);
        tiles.push_back(tile);
    }

    int i = -1;
    int j = 0;

    for (int k = 0; k < sizeX * sizeY; k++)
    {
        if (j % sizeY == 0)
        {
            i++;
        }
        tilemap[i][j++ % sizeY] = tiles[k];
    }
}

void Gpu::Draw()
{
    for (int i = 0; i < sizeX; i++)
    {
        for (int j = 0; j < sizeY; j++)
        {
            for (int k = 0; k < 8; k++)
            {
                for (int l = 0; l < 8; l++)
                {
                    gManager.Draw((i * 8) + k, (j * 8) + l, gManager.GetColor(tilemap[i][j].pixels[l][k]));
                }
            }
        }
    }

    gManager.Flush();
}
