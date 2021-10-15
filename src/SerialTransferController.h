#pragma once
#include "IMemoryMappable.h"
#include "InterruptController.h"

class SerialTransferController : public IMemoryMappable
{
private:
    Byte mem[(ADDR_SERIAL_END - ADDR_SERIAL_START) + 1];
    InterruptController* pInterruptController;

public:
    SerialTransferController(InterruptController* interruptController)
        : pInterruptController(interruptController)
    {}

    ~SerialTransferController() {}

    void Write(Address address, Byte value);
    Byte Read(Address address);
};