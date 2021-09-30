#pragma once
#include "IMemoryMappable.h"

class SoundController : public IMemoryMappable
{
private:
    Byte mem[ADDR_SOUND_END - ADDR_SOUND_START + 1];

public:
    bool On = false;

    void Write(Address address, Byte value);
    Byte Read(Address address);
};