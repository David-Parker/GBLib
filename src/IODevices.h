#pragma once
#include "InterruptController.h"
#include "SoundController.h"
#include "PixelProcessingUnit.h"
#include "Memory.h"

struct IODevices
{
public:
    InterruptController interruptController;
    SoundController soundController;
    PixelProcessingUnit ppu;

    IODevices(Memory* pMemory) : ppu(pMemory) {}
    ~IODevices() {}
};