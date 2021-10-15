#pragma once
#include "IMemoryMappable.h"
#include "InterruptController.h"

class SoundController : public IMemoryMappable
{
private:
    Byte mem[(ADDR_SOUND_END - ADDR_SOUND_START) + 1];
    InterruptController* pInterruptController;

public:
    bool On = false;

    SoundController(InterruptController* interruptController)
        : pInterruptController(interruptController)
    {}

    ~SoundController() {}

    void Write(Address address, Byte value);
    Byte Read(Address address);
};