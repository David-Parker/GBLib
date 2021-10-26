#pragma once
#include "IMemoryMappable.h"
#include "InterruptController.h"


enum SERIAL_TRANSFER_CONTROL_FLAGS
{
    TRANSFER_START = 128,
    CLOCK_SPEED = 2,
    SHIFT_CLOCK = 1
};

class SerialTransferController : public IMemoryMappable
{
private:
    Byte mem[(ADDR_SERIAL_END - ADDR_SERIAL_START) + 1];
    InterruptController* pInterruptController;
    RegisterU8 SB;
    RegisterU8 SC;

public:
    SerialTransferController(InterruptController* interruptController)
        : pInterruptController(interruptController), SB(&mem[0]), SC(&mem[1])
    {}

    ~SerialTransferController() {}

    void Write(Address address, Byte value);
    Byte Read(Address address);
};