#include "Gpu.h"

Gpu::Gpu()
{
	gManager.init();
}

Gpu::~Gpu()
{
}

void Gpu::LoadTileMap(const std::vector<Tile>& tiles)
{
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

void Gpu::Refresh()
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
