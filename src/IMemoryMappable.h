#pragma once
#include "Address.h"

// Denotes that a type may be mapped into a memory address range.
class IMemoryMappable
{
public:
    IMemoryMappable() {}
    virtual ~IMemoryMappable() {}

    virtual void Write(Address address, Byte value) = 0;
    virtual Byte Read(Address address) = 0;
};