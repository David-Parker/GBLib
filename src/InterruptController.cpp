#include "InterruptController.h"
#include <exception>

void InterruptController::Write(Address address, Byte value)
{
    if (address == ADDR_IF)
    {
        IF = value;
    }
    if (address == ADDR_IE)
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
    if (address == ADDR_IF)
    {
        return IF;
    }
    if (address == ADDR_IE)
    {
        return IE;
    }
    else
    {
        throw std::exception("Address is not valid.");
    }
}