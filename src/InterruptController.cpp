#include "InterruptController.h"
#include <exception>

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