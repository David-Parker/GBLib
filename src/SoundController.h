#pragma once
#include "IMemoryMappable.h"

class SoundController : public IMemoryMappable
{
private:
    Address offset = ADDR_NR10;
    Byte mem[21];

public:
    bool On = false;

    void Write(Address address, Byte value);
    Byte Read(Address address);
};