#include "SerialTransferController.h"

SerialTransferController::SerialTransferController()
{
}

SerialTransferController::~SerialTransferController()
{
}

void SerialTransferController::Write(Address address, Byte value)
{
    mem[address - ADDR_SERIAL_START] = value;
}

Byte SerialTransferController::Read(Address address)
{
    return mem[address - ADDR_SERIAL_START];
}