#include <iostream>
#include <string>
#include "GameBoy.h"
#include "Gpu.h"
#undef main

int main()
{
	GameBoy boy;
	boy.LoadRom("Tetris.gb");
	boy.GetGameInfo().PrintInfo();
	Gpu gpu;
	int tileCount = 0;
	std::vector<Tile> tiles;

	for (Address i = 0x3000; i < 0x7000; i++)
	{
		Byte bytes[16];

		for (int j = 0; j < 16; j++)
		{
			bytes[j] = boy.memory.ReadValue(i);

			if (j < 15) i++;
		}

		Tile tile = Tile(bytes);
		tiles.push_back(tile);
	}

	gpu.LoadTileMap(tiles);
	gpu.Refresh();


	//gManager.close();

	while (1);
}