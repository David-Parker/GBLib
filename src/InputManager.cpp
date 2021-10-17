#include "InputManager.h"
#include "SDL_events.h"
#include <cstdlib>

void InputManager::HandleEvents()
{
    SDL_Event event;

    while (SDL_PollEvent(&event)) {
        if (event.type == SDL_QUIT)
        {
            SDL_Quit();
            exit(EXIT_SUCCESS);
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
                case SDLK_RETURN:
                    this->pJoypadController->KeyDown(JOYPAD_BUTTONS::BUTTON_START);
                    break;
                case SDLK_DOWN:
                    this->pJoypadController->KeyDown(JOYPAD_BUTTONS::BUTTON_DOWN);
                    break;
                case SDLK_RSHIFT:
                    this->pJoypadController->KeyDown(JOYPAD_BUTTONS::BUTTON_SELECT);
                    break;
                case SDLK_UP:
                    this->pJoypadController->KeyDown(JOYPAD_BUTTONS::BUTTON_UP);
                    break;
                case SDLK_a:
                    this->pJoypadController->KeyDown(JOYPAD_BUTTONS::BUTTON_A);
                    break;
                case SDLK_RIGHT:
                    this->pJoypadController->KeyDown(JOYPAD_BUTTONS::BUTTON_RIGHT);
                    break;
                case SDLK_b:
                    this->pJoypadController->KeyDown(JOYPAD_BUTTONS::BUTTON_B);
                    break;
                case SDLK_LEFT:
                    this->pJoypadController->KeyDown(JOYPAD_BUTTONS::BUTTON_LEFT);
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
                case SDLK_RETURN:
                    this->pJoypadController->KeyUp(JOYPAD_BUTTONS::BUTTON_START);
                    break;
                case SDLK_DOWN:
                    this->pJoypadController->KeyUp(JOYPAD_BUTTONS::BUTTON_DOWN);
                    break;
                case SDLK_RSHIFT:
                    this->pJoypadController->KeyUp(JOYPAD_BUTTONS::BUTTON_SELECT);
                    break;
                case SDLK_UP:
                    this->pJoypadController->KeyUp(JOYPAD_BUTTONS::BUTTON_UP);
                    break;
                case SDLK_a:
                    this->pJoypadController->KeyUp(JOYPAD_BUTTONS::BUTTON_A);
                    break;
                case SDLK_RIGHT:
                    this->pJoypadController->KeyUp(JOYPAD_BUTTONS::BUTTON_RIGHT);
                    break;
                case SDLK_b:
                    this->pJoypadController->KeyUp(JOYPAD_BUTTONS::BUTTON_B);
                    break;
                case SDLK_LEFT:
                    this->pJoypadController->KeyUp(JOYPAD_BUTTONS::BUTTON_LEFT);
                    break;
            }
        }
    }
}