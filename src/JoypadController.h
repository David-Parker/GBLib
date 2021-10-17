#pragma once
#include "IMemoryMappable.h"
#include "InterruptController.h"
#include "GlobalDefinitions.h"

enum JOYPAD_FLAGS
{
    INPUT_SELECT_ACTION = 32,
    INPUT_SELECT_DIRECTION = 16,
    INPUT_DOWN_START = 8,
    INPUT_UP_SELECT = 4,
    INPUT_LEFT_B = 2,
    INPUT_RIGHT_A = 1
};

enum JOYPAD_BUTTONS
{
    BUTTON_A = 128,
    BUTTON_B = 64,
    BUTTON_START = 32,
    BUTTON_SELECT = 16,
    BUTTON_UP = 8,
    BUTTON_DOWN = 4,
    BUTTON_LEFT = 2,
    BUTTON_RIGHT = 1
};

class JoypadController : public IMemoryMappable
{
private:
    Byte reg;
    Byte buttons;
    RegisterU8 JOYP;
    RegisterU8 BUTTONS;
    InterruptController* pInterruptController;

public:
    JoypadController(InterruptController* interruptController)
        : pInterruptController(interruptController), JOYP(&reg), BUTTONS(&buttons)
    {
        this->JOYP = 0b00111111;
    }

    ~JoypadController() {}

    void Write(Address address, Byte value);
    Byte Read(Address address);
    void KeyDown(u8 key);
    void KeyUp(u8 key);
    void RequestButtonPress(u8 flags);
    void ClearButtonPress(u8 flags);
    bool DirectionSwitch();
    bool ActionSwitch();
};