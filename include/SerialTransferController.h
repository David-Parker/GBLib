#pragma once
#include "IMemoryMappable.h"
#include "InterruptController.h"
#include "ISerialHandler.h"

enum SERIAL_TRANSFER_CONTROL_FLAGS
{
    TRANSFER_START = 128,
    CLOCK_SPEED = 2,
    SHIFT_CLOCK = 1
};

class SerialTransferController : public IMemoryMappable
{
private:
    Byte mem[(ADDR_SERIAL_END - ADDR_SERIAL_START) + 1] = {0};
    InterruptController* pInterruptController;
    RegisterU8 SB;
    RegisterU8 SC;
    ISerialHandler* serialHandler;
    bool isMaster;
    u8 shiftsRemaining;
    Byte sentByte;
    Byte recievedByte;
    RegisterU16 shiftRegister;
    u64 clockCycles;

public:
    SerialTransferController(InterruptController* interruptController, ISerialHandler* serialHandler)
        : pInterruptController(interruptController), SB(&mem[0]), SC(&mem[1]), serialHandler(serialHandler), isMaster(false), shiftsRemaining(0), sentByte(0), recievedByte(0xFF), clockCycles(0), shiftRegister(0)
    {}

    ~SerialTransferController() {}

    void Write(Address address, Byte value);
    Byte Read(Address address);
    bool TransferRequested();
    void Tick(u64 cycles);
};