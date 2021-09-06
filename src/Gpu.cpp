#include "Gpu.h"

void Gpu::LoadTileMap(Address start, Address end)
{
	Tile tiles[sizeX * sizeY];
	Byte bytes[16];
	int tilesIdx = 0;

    for (Address i = start; i < end;)
    {
        for (int j = 0; j < 16; j++)
        {
            bytes[j] = pMemory->Read(i++);
        }

        Tile tile = Tile(bytes);
		tiles[tilesIdx++] = tile;
    }

    int i = -1;
    int j = 0;

    for (int k = 0; k < sizeX * sizeY; k++)
    {
        if (j % sizeY == 0)
        {
            i++;
        }

		tilemap[j++ % sizeY][i] = tiles[k];
		//tilemap[i][j++ % sizeY] = tiles[k];
    }
}

void Gpu::Draw()
{
	// Get Current tiles pointed by memory.

    for (int i = 0; i < TILE_HEIGHT; i++)
    {
        for (int j = 0; j < TILE_WIDTH; j++)
        {
            for (int k = 0; k < 8; k++)
            {
                for (int l = 0; l < 8; l++)
                {
					gManager.AddPixel((j * 8) + l, (i * 8) + k, tilemap[i][j].pixels[k][l]);
                }
            }
        }
    }

	gManager.Clear();
	gManager.Draw();
    gManager.Flush();
}
