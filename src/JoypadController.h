#pragma once
#include "IMemoryMappable.h"
#include "InterruptController.h"
#include "GlobalDefinitions.h"

class JoypadController : public IMemoryMappable
{
private:
    Byte reg;
    InterruptController* pInterruptController;

public:
    JoypadController(InterruptController* interruptController)
        : pInterruptController(interruptController)
    {}

    ~JoypadController() {}

    void Write(Address address, Byte value);
    Byte Read(Address address);
};