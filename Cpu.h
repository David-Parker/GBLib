#pragma once
#include "Address.h"
#include "Byte.h"

class Cpu
{
private:
	// Registers A-L
	Byte A;
	Byte B;
	Byte C;
	Byte D;
	Byte E;
	Byte F;
	Byte H;
	Byte L;
	Address SP; // Stack pointer
	Address PC; // Program counter
public:
	// On startup, stack pointer is set to 0xFFFE and the PC starts at the user defined code starting at address 0x100
	Cpu() : SP(0xFFFE), PC(0x100) {}
	~Cpu();
};

