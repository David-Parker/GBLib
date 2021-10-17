#pragma once
#include "SDL.h"
#include "Address.h"
#include "GlobalDefinitions.h"
#include "InterruptController.h"
#include "JoypadController.h"
#include "Memory.h"

class InputManager
{
private:
    Memory* pMemory;
    InterruptController* pInterruptController;
    JoypadController* pJoypadController;

public:
    InputManager(Memory* memory, InterruptController* interruptController, JoypadController* joypadController)
        : pMemory(memory), pInterruptController(interruptController), pJoypadController(joypadController)
    {
    }

    ~InputManager() {}

    void HandleEvents();
};