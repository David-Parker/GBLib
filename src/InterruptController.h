#pragma once
#include "IMemoryMappable.h"

class InterruptController : public IMemoryMappable
{
private:
    u8 IE; // Interrupt Enable
    u8 IF; // Interrupt Flag

public:
    InterruptController() {}
    ~InterruptController() {}

    void Write(Address address, Byte value);
    Byte Read(Address address);
};