#pragma once
#include <stdexcept>
#include <fstream>
#include <iostream>
#include <vector>
#include "Address.h"
#include "GlobalDefinitions.h"
#include "IMemoryMappable.h"
#include "RAM.h"

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

    std::vector<AddressRange> mappedIO;
    Byte MEM[Address::ADDRESSSPACE] = {};

    IMemoryMappable* IsMemoryMapped(Address address);

public:
    Memory();
    ~Memory();
    RAM vRAM;

    void ClearMemory();
    void Write(Address address, Byte value);
    Byte Read(Address address);
    void Dump(Address start, Address end);
    void MapMemory(Address low, Address high, IMemoryMappable* device);
    void UnMapMemory(Address address);
};