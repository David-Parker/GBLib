#pragma once
#include <vector>
#include "GlobalDefinitions.h"
#include "GraphicsManager.h"
#include "Memory.h"
#include "Tile.h"

class Gpu
{
private:
    Memory* pMemory;
    const static u16 sizeX = 32;
    const static u16 sizeY = 32;
    Tile tilemap[sizeX][sizeY];
    GraphicsManager gManager;

public:
    Gpu(Memory* pMemory) : pMemory(pMemory) 
    { 
        gManager.Init();
    }

    ~Gpu() {}

	void LoadTileMap(Address start, Address end);
    void Draw();
};

