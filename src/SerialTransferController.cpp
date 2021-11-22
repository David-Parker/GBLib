#include "SerialTransferController.h"
#include <iostream>

void SerialTransferController::Write(Address address, Byte value)
{
    if (address == ADDR_SERIAL_REG_SB)
    {
        SB = value;
    }
    else if (address == ADDR_SERIAL_REG_SC)
    {
        SC = value;

        if (TransferRequested())
        {
            // Serial transfer requested
            if (SC.FlagIsSet(SERIAL_TRANSFER_CONTROL_FLAGS::SHIFT_CLOCK))
            {
                // Send byte to secondary GameBoy.
                this->isMaster = true;

                if (this->serialHandler->IsSerialConnected())
                {
                    this->sentByte = SB;
                    this->serialHandler->SendByte(SB);
                    this->recievedByte = this->serialHandler->RecieveByte();
                }
                else
                {
                    this->sentByte = SB;
                    this->recievedByte = 0xFF;
                }

                SB = this->recievedByte;

                this->pInterruptController->RequestInterrupt(INTERRUPT_FLAGS::INT_SERIAL);
                SC.ClearFlags(SERIAL_TRANSFER_CONTROL_FLAGS::TRANSFER_START);

                //this->shiftRegister.SetHighByte(this->sentByte);
                //this->shiftRegister.SetLowByte(this->recievedByte);
                //this->shiftsRemaining = 8;
            }
        }
    }
    else
    {
        throw std::runtime_error("Address is not valid for the Serial Transfer Controller.");
    }
}

Byte SerialTransferController::Read(Address address)
{
    return mem[address - ADDR_SERIAL_START];
}

bool SerialTransferController::TransferRequested()
{
    return SC.FlagIsSet(SERIAL_TRANSFER_CONTROL_FLAGS::TRANSFER_START);
}

void SerialTransferController::Tick(u64 cycles)
{
    this->clockCycles += cycles * CLOCK_CYCLES_PER_MACHINE_CYCLE;

    if (this->shiftsRemaining > 0)
    {
        //this->shiftRegister = this->shiftRegister << 1;
        //this->shiftsRemaining--;

        //SB = this->shiftRegister.GetHighByte();

        //if (this->shiftsRemaining == 0)
        //{
        //    this->pInterruptController->RequestInterrupt(INTERRUPT_FLAGS::INT_SERIAL);
        //    SC.ClearFlags(SERIAL_TRANSFER_CONTROL_FLAGS::TRANSFER_START);
        //}

        //this->shiftsRemaining = 0;
        //this->pInterruptController->RequestInterrupt(INTERRUPT_FLAGS::INT_SERIAL);
        //SC.ClearFlags(SERIAL_TRANSFER_CONTROL_FLAGS::TRANSFER_START);
    }
    // Check if a master GameBoy sent us a byte.
    if (this->serialHandler->ByteRecieved())
    {
        this->isMaster = false;

        if (this->serialHandler->IsSerialConnected())
        {
            this->recievedByte = this->serialHandler->RecieveByte();
            this->sentByte = SB;
            this->serialHandler->SendByte(SB);
        }
        else
        {
            this->sentByte = SB;
            this->recievedByte = 0xFF;
        }

        //this->shiftRegister.SetHighByte(this->sentByte);
        //this->shiftRegister.SetLowByte(this->recievedByte);
        //this->shiftsRemaining = 8;

        SB = this->recievedByte;
        this->pInterruptController->RequestInterrupt(INTERRUPT_FLAGS::INT_SERIAL);
        SC.ClearFlags(SERIAL_TRANSFER_CONTROL_FLAGS::TRANSFER_START);
    }

    if (this->clockCycles >= CLOCKS_PER_SERIAL_TICK)
    {
        this->clockCycles -= CLOCKS_PER_SERIAL_TICK;

        // Check if current bit transfer simulation is in progress.
        //if (this->shiftsRemaining > 0)
        //{
        //    this->shiftRegister = this->shiftRegister << 1;
        //    this->shiftsRemaining--;

        //    SB = this->shiftRegister.GetHighByte();

        //    if (this->shiftsRemaining == 0)
        //    {
        //        this->pInterruptController->RequestInterrupt(INTERRUPT_FLAGS::INT_SERIAL);
        //        SC.ClearFlags(SERIAL_TRANSFER_CONTROL_FLAGS::TRANSFER_START);
        //    }
        //}
        //else
        //{
        //    // Check if a master GameBoy sent us a byte.
        //    if (this->serialHandler->ByteRecieved())
        //    {
        //        this->isMaster = false;

        //        if (this->serialHandler->IsSerialConnected())
        //        {
        //            this->recievedByte = this->serialHandler->RecieveByte();
        //            this->sentByte = SB;
        //            this->serialHandler->SendByte(SB);
        //        }
        //        else
        //        {
        //            this->sentByte = SB;
        //            this->recievedByte = 0xFF;
        //        }

        //        //this->shiftRegister.SetHighByte(this->sentByte);
        //        //this->shiftRegister.SetLowByte(this->recievedByte);
        //        //this->shiftsRemaining = 8;

        //        SB = this->recievedByte;
        //        this->pInterruptController->RequestInterrupt(INTERRUPT_FLAGS::INT_SERIAL);
        //        SC.ClearFlags(SERIAL_TRANSFER_CONTROL_FLAGS::TRANSFER_START);
        //    }
        //}
    }
}