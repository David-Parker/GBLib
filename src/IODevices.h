#pragma once
#include "InterruptController.h"
#include "IGraphicsHandler.h"
#include "SoundController.h"
#include "PixelProcessingUnit.h"
#include "JoypadController.h"
#include "SerialTransferController.h"
#include "TimerController.h"
#include "Memory.h"

struct IODevices
{
public:
    InterruptController interruptController;
    SoundController soundController;
    PixelProcessingUnit ppu;
    JoypadController joypadController;
    SerialTransferController serialController;
    TimerController timerController;

    IODevices(Memory* pMemory, IGraphicsHandler* graphicsHandler, IEventHandler* eventHandler) 
    : 
        interruptController(),
        soundController(&interruptController),
        joypadController(&interruptController),
        ppu(pMemory, &interruptController, &joypadController, graphicsHandler, eventHandler),
        serialController(&interruptController),
        timerController(&interruptController)
    {
    }

    ~IODevices() {}
};