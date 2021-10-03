#include "Tile.h"
#include <cstring>
#include <random>

void Tile::LoadTileData(Memory* pMemory, Address data)
{
    this->pMemory = pMemory;
    this->data = data;
}