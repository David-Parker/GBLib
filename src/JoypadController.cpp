#include "JoypadController.h"

void JoypadController::Write(Address address, Byte value)
{
    Byte upper = 0b00110000 & value;
    Byte lower = 0b00001111 & JOYP;

    JOYP = upper + lower;
}

Byte JoypadController::Read(Address address)
{
    if (DirectionSwitch())
    {
        BUTTONS.FlagIsSet(BUTTON_UP) ? JOYP.ClearFlags(JOYPAD_FLAGS::INPUT_UP_SELECT) : JOYP.SetFlags(JOYPAD_FLAGS::INPUT_UP_SELECT);
        BUTTONS.FlagIsSet(BUTTON_DOWN) ? JOYP.ClearFlags(JOYPAD_FLAGS::INPUT_DOWN_START) : JOYP.SetFlags(JOYPAD_FLAGS::INPUT_DOWN_START);
        BUTTONS.FlagIsSet(BUTTON_LEFT) ? JOYP.ClearFlags(JOYPAD_FLAGS::INPUT_LEFT_B) : JOYP.SetFlags(JOYPAD_FLAGS::INPUT_LEFT_B);
        BUTTONS.FlagIsSet(BUTTON_RIGHT) ? JOYP.ClearFlags(JOYPAD_FLAGS::INPUT_RIGHT_A) : JOYP.SetFlags(JOYPAD_FLAGS::INPUT_RIGHT_A);
    }
    
    if (ActionSwitch())
    {
        BUTTONS.FlagIsSet(BUTTON_SELECT) ? JOYP.ClearFlags(JOYPAD_FLAGS::INPUT_UP_SELECT) : JOYP.SetFlags(JOYPAD_FLAGS::INPUT_UP_SELECT);
        BUTTONS.FlagIsSet(BUTTON_START) ? JOYP.ClearFlags(JOYPAD_FLAGS::INPUT_DOWN_START) : JOYP.SetFlags(JOYPAD_FLAGS::INPUT_DOWN_START);
        BUTTONS.FlagIsSet(BUTTON_B) ? JOYP.ClearFlags(JOYPAD_FLAGS::INPUT_LEFT_B) : JOYP.SetFlags(JOYPAD_FLAGS::INPUT_LEFT_B);
        BUTTONS.FlagIsSet(BUTTON_A) ? JOYP.ClearFlags(JOYPAD_FLAGS::INPUT_RIGHT_A) : JOYP.SetFlags(JOYPAD_FLAGS::INPUT_RIGHT_A);
    }

    return JOYP;
}

bool JoypadController::DirectionSwitch()
{
    return !JOYP.FlagIsSet(JOYPAD_FLAGS::INPUT_SELECT_DIRECTION);
}

bool JoypadController::ActionSwitch()
{
    return !JOYP.FlagIsSet(JOYPAD_FLAGS::INPUT_SELECT_ACTION);
}

void JoypadController::KeyDown(u8 key)
{
    if (ActionSwitch() || DirectionSwitch())
    {
        bool keyIsUp = !BUTTONS.FlagIsSet(key);

        if (keyIsUp)
        {
            this->pInterruptController->RequestInterrupt(INTERRUPT_FLAGS::INT_JOYPAD);
        }
    }

    BUTTONS.SetFlags(key);
}

void JoypadController::KeyUp(u8 key)
{
    BUTTONS.ClearFlags(key);
}