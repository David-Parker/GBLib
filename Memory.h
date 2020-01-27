#pragma once
#include <stdexcept>
#include <fstream>
#include <iostream>
#include "Address.h"
#include "GlobalDefinitions.h"

class Memory
{
private:
	Byte memory[Address::ADDRESSSPACE] = {};
public:
	Memory();
	~Memory();
	void ClearMemory();
	void StoreValue(Address address, Byte value);
	Byte ReadValue(Address address);
	void Dump(Address start, Address end);
};

