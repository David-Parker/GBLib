#pragma once
#include "InterruptController.h"
#include "SoundController.h"
#include "PixelProcessingUnit.h"
#include "Memory.h"

class IODevices
{
public:
    InterruptController interruptController;
    SoundController soundController;
    PixelProcessingUnit ppu;

    IODevices(Memory* pMemory) : ppu(pMemory) {}
    ~IODevices() {}
};