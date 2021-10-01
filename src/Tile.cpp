#include "Tile.h"
#include <cstring>
#include <random>

void Tile::LoadData(Byte data[16])
{
    memcpy(this->data, data, 16);
}