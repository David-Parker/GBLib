#pragma once

#include "GlobalDefinitions.h"
#include "InterruptController.h"
#include "JoypadController.h"

// Processes input events to send to the joypad controller.
class IEventHandler
{
public:
    // Polls for input events, and sets the appropriate button presses on the joypad controller.
    virtual void HandleInput(JoypadController* joypadController) = 0;

    // Should the emulator quit?
    virtual bool ShouldQuit() = 0;

    // Indicates what speed multiplier the emulator should actively run at.
    virtual int SpeedMultiplier() = 0;
};