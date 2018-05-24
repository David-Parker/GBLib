#pragma once
#include <vector>
#include "GraphicsManager.h"
#include "Tile.h"

class Gpu
{
private:
	const static int sizeX = 32;
	const static int sizeY = 32;
	Tile tilemap[sizeX][sizeY];
	GraphicsManager gManager;
public:
	Gpu();
	~Gpu();
	void LoadTileMap(const std::vector<Tile>& tiles);
	void Refresh();
};

