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
	for (int i = 0 ; i < Address::ADDRESSSPACE; ++i)
	{
		memory[i] = Byte();
	}
}

void Memory::LoadValue(Address address, Byte value)
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

void Memory::MemoryDump(Address start, Address end)
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

	for (Address i = start; i < end; i++)
	{
		myfile << "0x" << std::setfill('0') << std::setw(4) << std::hex << i << ": " << ReadValue(i).ToString() << std::endl;
	}
}
