#include "Tile.h"
#include <cstring>
#include <random>

Tile::Tile(Memory* pMemory, Address data)
    : pMemory(pMemory), data(data)
{
}