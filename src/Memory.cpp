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

void Memory::Write(Address address, Byte value)
{
    if (address >= Address::ADDRESSSPACE)
    {
        throw std::invalid_argument("The provided address exceeds the size of addressable memory.");
    }

    memory[address] = value;
}

Byte Memory::Read(Address address)
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

	FILE *file;
	errno_t err;

	if ((err = fopen_s(&file, "dump.txt", "wb")) != 0)
	{
		throw std::exception("Could not open ROM file.");
	}
	else
	{
		for (u32 i = start; i <= end; i++)
		{
			fprintf(file, "[0x%04X]: 0x%04X\n", i, Read((Address)i));
		}

		fclose(file);
	}
}
