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
    u16 Combine(u8 high, u8 low);

#pragma region Instructions
    // 8-bit Transfer and I/O                      // opcode   operands   cycles
    int Ld(RegisterU8& dest, RegisterU8& src);     // 01xxxyyy (r, r) -> 1
    int Ld(RegisterU8& dest, u8 value);            // 00xxx110 (r) + xxxxxxxx (n) -> 2
    int LdrHL(RegisterU8& src);                    // 01xxx110 (r) -> 2
    int LdHLr(RegisterU8& src);                    // 01110xxx (r) -> 2
    int LdHLr(u8 value);                           // 00110110 + xxxxxxxx (n) -> 3
    int LdABC();                                   // 00001010 -> 2
    int LdADE();                                   // 00011010 -> 2
    int LdAC();                                    // 11110010 -> 2
    int LdCA();                                    // 11100010 -> 2
    int LdAn(u8 value);                            // 11110000 + xxxxxxxx (n) -> 3
    int LdnA(u8 value);                            // 11100000 + xxxxxxxx (n) -> 3
    int LdAnn(u16 value);                          // 11111010 + xxxxxxxx (n) + xxxxxxxx (n) -> 4
    int LdnnA(u16 value);                          // 11101010 + xxxxxxxx (n) + xxxxxxxx (n) -> 4

    // 8-bit Arithmetic
    int Add(RegisterU8& reg);                      // 10000xxx (r) -> 1
    int Add(u8 value);                             // 11000110 + xxxxxxx (n) -> 2
    int AddHL();                                   // 10000110 -> 2
    int Adc(RegisterU8& reg);                      // 10001xxx (r) -> 1
    int Adc(u8 value);                             // 11001110 + xxxxxxxx (n) -> 2
    int AdcHL();                                   // 10001110 -> 2
    int Sub(RegisterU8& reg);                      // 10010xxx (r) -> 1
    int Sub(u8 value);                             // 11010110 + xxxxxxxx (n) -> 2
    int SubHL();                                   // 10010110 -> 2
    int Sbc(RegisterU8& reg);                      // 10011xxx (r) -> 1
    int Sbc(u8 value);                             // 11011110 + xxxxxxx (n) -> 2
    int SbcHL();                                   // 10011110 -> 2

    // 8-bit Logical Operators
    int And(RegisterU8& reg);                      // 10100xxx (r) -> 1
    int And(u8 value);                             // 11100110 + xxxxxxxx (n) -> 2
    int AndHL();                                   // 10100110 -> 2
    int Or(RegisterU8& reg);                       // 10110xxx (r) -> 1
    int Or(u8 value);                              // 11110110 + xxxxxxxx (n) -> 2
    int OrHL();                                    // 10110110 -> 2
    int Xor(RegisterU8& reg);                      // 10101xxx (r) -> 1
    int Xor(u8 value);                             // 11101110 + xxxxxxxx (n) -> 2
    int XorHL();                                   // 10101110 -> 2
    int Cmp(RegisterU8& reg);                      // 10111xxx (r) -> 1
    int Cmp(u8 value);                             // 11111110 + xxxxxxxx (n) -> 2
    int CmpHL();                                   // 10111110 -> 2
    int Inc(RegisterU8& reg);                      // 00xxx100 (r) -> 1
    int IncHL();                                   // 00110100 -> 3
    int Dec(RegisterU8& reg);                      // 00xxx101 (r) -> 1
    int DecHL();                                   // 00110101 -> 3
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

