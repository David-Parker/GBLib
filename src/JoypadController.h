#pragma once
#include "IMemoryMappable.h"
#include "GlobalDefinitions.h"

class JoypadController :
    public IMemoryMappable
{
private:
    Byte reg;

public:
    JoypadController();
    ~JoypadController();

    void Write(Address address, Byte value);
    Byte Read(Address address);
};