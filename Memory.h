#pragma once
#include <stdexcept>
#include <fstream>
#include <iostream>
#include "Byte.h"
#include "Address.h"

class Memory
{
private:
	Byte memory[Address::ADDRESSSPACE] = {};
public:
	Memory();
	~Memory();
	void ClearMemory();
	void LoadValue(Address address, Byte value);
	Byte ReadValue(Address address);
	void MemoryDump(Address start, Address end);
};

