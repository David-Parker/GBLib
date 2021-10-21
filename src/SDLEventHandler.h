#pragma once
#include "IEventHandler.h"

class SDLEventHandler :
    public IEventHandler
{
public:
    SDLEventHandler();
    ~SDLEventHandler();

    void HandleInput(JoypadController* joypadController);
};