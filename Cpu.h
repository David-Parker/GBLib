#pragma once
#include "Address.h"
#include "GlobalDefinitions.h"
#include "Memory.h"
#include "RegisterU8.h"
#include "RegisterU16.h"

#define CLOCK_HZ 4194304
#define CLOCK_NS_PER_CYCLE 238
#define CLOCK_CYCLES_PER_MACHINE_CYCLE 4

class Cpu
{
private:
	// Memory
	Memory* pMemory;

	// Registers
	RegisterU16 AF; // Accumulator & Flags
	RegisterU16 BC; 
	RegisterU16 DE;
	RegisterU16 HL;
	RegisterU16 SP; // Stack Pointer
	RegisterU16 PC; // Program Counter
	RegisterU8 A; // Accumulator
	RegisterU8 B;
	RegisterU8 C;
	RegisterU8 D;
	RegisterU8 E;
	RegisterU8 F; // Flags
	RegisterU8 H;
	RegisterU8 L;

	// Helpers
	int AddCommon(u8 value, bool addCarry);
	int SubCommon(u8 value, bool subCarry);

#pragma region Instructions
	// 8-bit arithmetic
	int Add(RegisterU8& reg);
	int Add(u8 value);
	int Adc(RegisterU8& reg);
	int Adc(u8 value);
	int Sub(RegisterU8& reg);
	int Sub(u8 value);
	int Sbc(RegisterU8& reg);
	int Sbc(u8 value);

	// 8-bit logical operators
	int And(RegisterU8& reg);
	int And(u8 value);
	int Or(RegisterU8& reg);
	int Or(u8 value);
	int Xor(RegisterU8& reg);
	int Xor(u8 value);
	int Cmp(RegisterU8& reg);
	int Cmp(u8 value);
	int Inc(RegisterU8& reg);
	int IncHL();
	int Dec(RegisterU8& reg);
	int DecHL();
#pragma endregion Instructions

public:
	// On startup, stack pointer is set to 0xFFFE and the PC starts at the user defined code starting at address 0x100
	Cpu(Memory* pMemory) : SP(0xFFFE), PC(0x100), pMemory(pMemory),
		A((u8*)&AF + 1), B((u8*)&BC + 1), C((u8*)&BC), D((u8*)&DE + 1),
		E((u8*)&DE), F((u8*)&AF), H((u8*)&HL + 1), L((u8*)&HL) {}
	~Cpu();

	// Returns the amount of machine cycles to perform instruction
	int Tick();
};

