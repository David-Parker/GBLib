#include "TimerController.h"

TimerController::TimerController()
{
}

TimerController::~TimerController()
{
}

void TimerController::Write(Address address, Byte value)
{
    mem[address - ADDR_TIMER_START] = value;
}

Byte TimerController::Read(Address address)
{
    return mem[address - ADDR_TIMER_START];
}