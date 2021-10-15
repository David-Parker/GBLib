#pragma once
#include "IMemoryMappable.h"
#include "GlobalDefinitions.h"

class UnMappedMemory : public IMemoryMappable
{
public:
    UnMappedMemory();
    ~UnMappedMemory();

    void Write(Address address, Byte value);
    Byte Read(Address address);
};