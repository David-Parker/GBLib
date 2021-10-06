#pragma once
#include "IMemoryMappable.h"
class TimerController :
    public IMemoryMappable
{
private:
    Byte mem[(ADDR_TIMER_END - ADDR_TIMER_START) + 1];

public:
    TimerController();
    ~TimerController();

    void Write(Address address, Byte value);
    Byte Read(Address address);
};