#pragma once
#include "IMemoryMappable.h"
#include "GlobalDefinitions.h"

class RAM : public IMemoryMappable
{
private:
    Byte* mem;
    bool canAccess;
    Address start;
    Address end;

public:
    RAM(Address start, Address end);
    ~RAM();

    void Write(Address address, Byte value);
    __forceinline Byte Read(Address address);
    void EnableAccess();
    void DisableAccess();
};

 Byte RAM::Read(Address address)
{
    if (!this->canAccess)
    {
        return INVALID_READ;
    }

    return this->mem[address - this->start];
}