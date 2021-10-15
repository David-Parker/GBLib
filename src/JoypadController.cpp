#include "JoypadController.h"

void JoypadController::Write(Address address, Byte value)
{
    reg = value;
}

Byte JoypadController::Read(Address address)
{
    return reg;
}