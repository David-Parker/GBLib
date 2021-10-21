#pragma once

#include "GlobalDefinitions.h"
#include "InterruptController.h"
#include "JoypadController.h"

// Processes keyboard events to send to the joypad controller.
class IEventHandler
{
public:

    // Polls keyboard for input events, and sets the appropriate button presses on the joypad controller.
    virtual void HandleInput(JoypadController* joypadController) = 0;
};