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

void Memory::Dump(Address start, Address end)
{
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
