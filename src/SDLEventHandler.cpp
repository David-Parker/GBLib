#include "SDLEventHandler.h"
#include "SDL.h"
#include "SDL_events.h"

SDLEventHandler::~SDLEventHandler()
{
}

void SDLEventHandler::HandleInput(JoypadController* joypadController)
{
    SDL_Event event;

    while (SDL_PollEvent(&event)) {
        if (event.type == SDL_QUIT)
        {
            this->shouldQuit = true;
        }
        else if (event.type == SDL_KEYDOWN)
        {
            SDL_KeyboardEvent keyEvent = event.key;

            if (keyEvent.repeat)
            {
                continue;
            }

            switch (keyEvent.keysym.sym)
            {
            case SDLK_LSHIFT:
                this->speedMultiplier = 12;
                break;
            case SDLK_RETURN:
                joypadController->KeyDown(JOYPAD_BUTTONS::BUTTON_START);
                break;
            case SDLK_DOWN:
                joypadController->KeyDown(JOYPAD_BUTTONS::BUTTON_DOWN);
                break;
            case SDLK_RSHIFT:
                joypadController->KeyDown(JOYPAD_BUTTONS::BUTTON_SELECT);
                break;
            case SDLK_UP:
                joypadController->KeyDown(JOYPAD_BUTTONS::BUTTON_UP);
                break;
            case SDLK_a:
                joypadController->KeyDown(JOYPAD_BUTTONS::BUTTON_A);
                break;
            case SDLK_RIGHT:
                joypadController->KeyDown(JOYPAD_BUTTONS::BUTTON_RIGHT);
                break;
            case SDLK_b:
                joypadController->KeyDown(JOYPAD_BUTTONS::BUTTON_B);
                break;
            case SDLK_LEFT:
                joypadController->KeyDown(JOYPAD_BUTTONS::BUTTON_LEFT);
                break;
            }
        }
        else if (event.type == SDL_KEYUP)
        {
            SDL_KeyboardEvent keyEvent = event.key;

            if (keyEvent.repeat)
            {
                continue;
            }

            switch (keyEvent.keysym.sym)
            {
            case SDLK_LSHIFT:
                this->speedMultiplier = 1;
                break;
            case SDLK_RETURN:
                joypadController->KeyUp(JOYPAD_BUTTONS::BUTTON_START);
                break;
            case SDLK_DOWN:
                joypadController->KeyUp(JOYPAD_BUTTONS::BUTTON_DOWN);
                break;
            case SDLK_RSHIFT:
                joypadController->KeyUp(JOYPAD_BUTTONS::BUTTON_SELECT);
                break;
            case SDLK_UP:
                joypadController->KeyUp(JOYPAD_BUTTONS::BUTTON_UP);
                break;
            case SDLK_a:
                joypadController->KeyUp(JOYPAD_BUTTONS::BUTTON_A);
                break;
            case SDLK_RIGHT:
                joypadController->KeyUp(JOYPAD_BUTTONS::BUTTON_RIGHT);
                break;
            case SDLK_b:
                joypadController->KeyUp(JOYPAD_BUTTONS::BUTTON_B);
                break;
            case SDLK_LEFT:
                joypadController->KeyUp(JOYPAD_BUTTONS::BUTTON_LEFT);
                break;
            }
        }
    }
}

bool SDLEventHandler::ShouldQuit()
{
    return this->shouldQuit;
}

int SDLEventHandler::SpeedMultiplier()
{
    return this->speedMultiplier;
}