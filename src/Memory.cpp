#include <iostream>
#include <iomanip>
#include "Memory.h"

Memory::Memory()
{
}

Memory::~Memory()
{
}

void Memory::ClearMemory()
{
    for (u32 i = 0 ; i < Address::ADDRESSSPACE; ++i)
    {
        memory[i] = 0;
    }
}

void Memory::StoreValue(Address address, Byte value)
{
    if (address >= Address::ADDRESSSPACE)
    {
        throw std::invalid_argument("The provided address exceeds the size of addressable memory.");
    }

    memory[address] = value;
}

Byte Memory::ReadValue(Address address)
{
    if (address >= Address::ADDRESSSPACE)
    {
        throw std::invalid_argument("The provided address exceeds the size of addressable memory.");
    }

    return memory[address];
}

void Memory::Dump(Address start, Address end)
{
    if (start >= Address::ADDRESSSPACE)
    {
        throw std::invalid_argument("The provided address exceeds the size of addressable memory.");
    }

    if (end >= Address::ADDRESSSPACE)
    {
        throw std::invalid_argument("The provided address exceeds the size of addressable memory.");
    }

    std::ofstream myfile;
    myfile.open("dump.txt");

    for (u32 i = start; i <= end; i++)
    {
        myfile << "0x" << std::setfill('0') << std::setw(4) << std::hex << i << ": " << ReadValue((Address)i) << std::endl;
    }
}
