#pragma once

#include "GlobalDefinitions.h"

// Sends bytes from a master GameBoy using an internal clock to an secondary GameBoy.
// Connection of virtual serial cable between two GameBoys should happen asyncronously
// on a seperate thread from that of the GameBoy emulator.
class ISerialHandler
{
public:
    // Returns true if a virtual "serial cable" is connected to this GameBoy.
    virtual bool IsSerialConnected() = 0;

    // Sends a byte to the other GameBoy.
    virtual void SendByte(Byte byte) = 0;

    // Returns true if a byte was recieved from another GameBoy.
    virtual bool ByteRecieved() = 0;

    // Blocks, waiting to recieve a new byte from other GameBoy.
    virtual Byte RecieveByte() = 0;
};