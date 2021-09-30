#include "SoundController.h"

void SoundController::Write(Address address, Byte value)
{
    mem[address - ADDR_SOUND_START] = value;
}

Byte SoundController::Read(Address address)
{
    return mem[address - ADDR_SOUND_START];
}