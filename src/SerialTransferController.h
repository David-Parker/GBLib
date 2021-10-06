#pragma once
#include "IMemoryMappable.h"

class SerialTransferController :
    public IMemoryMappable
{
private:
    Byte mem[(ADDR_SERIAL_END - ADDR_SERIAL_START) + 1];

public:
    SerialTransferController();
    ~SerialTransferController();

    void Write(Address address, Byte value);
    Byte Read(Address address);
};