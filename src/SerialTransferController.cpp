#include "SerialTransferController.h"
#include <iostream>

void SerialTransferController::Write(Address address, Byte value)
{
    mem[address - ADDR_SERIAL_START] = value;

    // Debug to serial output
    if (address == 0xFF02 && value == 0x81) {
        std::cout << this->Read(0xFF01);
    }
}

Byte SerialTransferController::Read(Address address)
{
    return mem[address - ADDR_SERIAL_START];
}