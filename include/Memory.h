#pragma once
#include <stdexcept>
#include <fstream>
#include <iostream>
#include <vector>
#include "Address.h"
#include "EMUType.h"
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
    RAM eRAM; // External RAM
    RAM oRAM; // OAM RAM
    RAM hRAM; // High RAM
    std::vector<RAM*> vRAMBanks; // Video RAM, bank 0 (8kb) is always used in DMG
    std::vector<RAM*> wRAMBanks; // Work RAM, bank 0 (4kb) and bank 1 (4kb) are always used in DMG

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