#pragma once
#include <stdexcept>
#include <fstream>
#include <iostream>
#include <vector>
#include "Address.h"
#include "GlobalDefinitions.h"
#include "IMemoryMappable.h"
#include "RAM.h"
#include "UnMappedMemory.h"

class Memory
{
private:
    struct AddressRange
    {
        AddressRange(Address low, Address high, IMemoryMappable* device)
            : low(low), high(high), device(device) {}

        Address low;
        Address high;
        IMemoryMappable* device;
    };

    UnMappedMemory unMapped;
    IMemoryMappable* addressSpace[ADDRESSSPACE] = {0};
    Byte cartrigeType;

public:
    Memory();
    ~Memory();
    RAM vRAM; // Video RAM
    RAM eRAM; // External RAM
    RAM gRAM; // General Purpose RAM
    RAM oRAM; // OAM RAM
    RAM hRAM; // High RAM

    void ClearMemory();
    void Write(Address address, Byte value);
    __inline Byte Read(Address address);
    void Dump(Address start, Address end);
    void MapMemory(Address low, Address high, IMemoryMappable* device);
    void UnMapMemory(Address low, Address high);
};

Byte Memory::Read(Address address)
{
    return this->addressSpace[address]->Read(address);
}