#pragma once
#include "IMemoryMappable.h"
#include "InterruptController.h"

enum TIMER_FLAGS
{
    TIMER_ENABLED = 4,
    CLOCK_FREQ_UPPER = 2,
    CLOCK_FREQ_LOWER = 1
};

class TimerController : public IMemoryMappable
{
private:
    Byte mem[(ADDR_TIMER_END - ADDR_TIMER_START) + 1];
    InterruptController* pInterruptController;
    u64 clockCyclesDiv;
    u64 clockCyclesTimer;

    RegisterU8 DIV;
    RegisterU8 TIMA;
    RegisterU8 TMA;
    RegisterU8 TAC;

    bool TimerEnabled();
    int GetTimerFrequencyClocks();

public:
    TimerController(InterruptController* interruptController)
        : 
        pInterruptController(interruptController),
        clockCyclesDiv(0),
        clockCyclesTimer(0),
        DIV(&mem[0]),
        TIMA(&mem[1]),
        TMA(&mem[2]),
        TAC(&mem[3])
    {}

    ~TimerController() {}

    void Write(Address address, Byte value);
    Byte Read(Address address);
    void Tick(u64 cycles);
};