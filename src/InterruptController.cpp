#include "InterruptController.h"
#include <exception>

InterruptController::InterruptController()
    : IE(&mem[0]), IF(&mem[1])
{
    this->IE = 0x00;
    this->IF = 0xE0;
}

void InterruptController::Write(Address address, Byte value)
{
    if (address == ADDR_INTERRUPT_FLAG)
    {
        IF = value;
    }
    else if (address == ADDR_INTERRUPT_ENABLE)
    {
        IE = value;
    }
    else
    {
        throw std::exception("Address is not valid.");
    }
}

Byte InterruptController::Read(Address address)
{
    if (address == ADDR_INTERRUPT_FLAG)
    {
        return IF;
    }
    else if (address == ADDR_INTERRUPT_ENABLE)
    {
        return IE;
    }
    else
    {
        throw std::exception("Address is not valid.");
    }
}

void InterruptController::RequestInterrupt(u8 flags)
{
    IF.SetFlags(flags);
}

void InterruptController::ClearInterruptRequest(u8 flags)
{
    IF.ClearFlags(flags);
}

void InterruptController::EnableInterrupt(u8 flags)
{
    IE.SetFlags(flags);
}

void InterruptController::DisableInterrupt(u8 flags)
{
    IE.ClearFlags(flags);
}

void InterruptController::ClearAllRequests()
{
    IF.ClearAllFlags();
}

void InterruptController::DisableAllInterrupts()
{
    IE.ClearAllFlags();
}

bool InterruptController::InterruptIsRequested(u8 flags)
{
    return IF.FlagIsSet(flags);
}

bool InterruptController::InterruptIsEnabled(u8 flags)
{
    return IE.FlagIsSet(flags);
}