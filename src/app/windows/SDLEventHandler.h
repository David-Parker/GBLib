#pragma once
#include "IEventHandler.h"

class SDLEventHandler :
    public IEventHandler
{
private:
    bool shouldQuit;
    int speedMultiplier;
public:
    SDLEventHandler()
        : shouldQuit(false), speedMultiplier(SPEED_MULTIPLIER)
    {
    }

    ~SDLEventHandler();

    void HandleInput(JoypadController* joypadController);
    bool ShouldQuit();
    int SpeedMultiplier();
};