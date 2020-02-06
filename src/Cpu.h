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
    bool CarryU8(u8 lhs, u8 rhs);
    bool HCarryU8(u8 lhs, u8 rhs);
    bool CarryU16(u16 lhs, u16 rhs);
    bool HCarryU16(u16 lhs, u16 rhs);
    bool CarryS8(u16 lhs, s8 rhs);
    bool HCarryS8(u16 lhs, s8 rhs);

#pragma region Instructions
    // 8-bit Transfer and I/O                       // opcode   operands   cycles
    int Nop();                                      // 00000000 -> 0
    int Ld(RegisterU8& dest, RegisterU8& src);      // 01xxxyyy (r, r) -> 1
    int Ld(RegisterU8& dest, u8 value);             // 00xxx110 (r) + xxxxxxxx (n) -> 2
    int LdrHL(RegisterU8& src);                     // 01xxx110 (r) -> 2
    int LdHLr(RegisterU8& src);                     // 01110xxx (r) -> 2
    int LdHLr(u8 value);                            // 00110110 + xxxxxxxx (n) -> 3
    int LdABC();                                    // 00001010 -> 2
    int LdADE();                                    // 00011010 -> 2
    int LdAC();                                     // 11110010 -> 2
    int LdCA();                                     // 11100010 -> 2
    int LdAn(u8 value);                             // 11110000 + xxxxxxxx (n) -> 3
    int LdnA(u8 value);                             // 11100000 + xxxxxxxx (n) -> 3
    int LdAnn(u16 value);                           // 11111010 + xxxxxxxx (n) + xxxxxxxx (n) -> 4
    int LdnnA(u16 value);                           // 11101010 + xxxxxxxx (n) + xxxxxxxx (n) -> 4
    int LdAHLI();                                   // 00101010 -> 2
    int LdAHLD();                                   // 00111010 -> 2
    int LdBCA();                                    // 00000010 -> 2
    int LdDEA();                                    // 00010010 -> 2
    int LdHLIA();                                   // 00100010 -> 2
    int LdHLDA();                                   // 00110010 -> 2

    // 16-bit Transfer
    int Ldnn(RegisterU16& dest, u16 value);         // 00xx0001 (dd) + xxxxxxxx (n) + xxxxxxxx (n) -> 3
    int LdSPHL();                                   // 11111001 -> 2
    int Push(RegisterU16& reg);                     // 11xx0101 (qq) -> 4
    int Pop(RegisterU16& reg);                      // 11xx0001 (qq) -> 3
    int LdHLSPe(s8 value);                          // 11111000 + xxxxxxxx (e) -> 3
    int LdnnSP(u16 value);                          // 00001000 + xxxxxxxx (n) + xxxxxxxx (n) -> 5

    // 16-bit Arithmetic
    int AddHL(RegisterU16& reg);                    // 00xx1001 (ss) -> 2
    int AddSP(s8 value);                            // 11101000 + xxxxxxxx (e) -> 4
    int Inc(RegisterU16& reg);                      // 00xx0011 (ss) -> 2
    int Dec(RegisterU16& reg);                      // 00xx1011 (ss) -> 2
    int RlcA();                                     // 00000111 -> 1
    int RlA();                                      // 00010111-> 1
    int RrcA();                                     // 00001111 -> 1
    int RrA();                                      // 00011111 -> 1
    int Rlc(RegisterU8& reg);                       // 
    int RlcHL();
    int Rl(RegisterU8& reg);
    int RlHL();
    int Rrc(RegisterU8& reg);
    int RrcHL();
    int Rr(RegisterU8& reg);
    int RrHL();
    int Sla(RegisterU8& reg);
    int SlaHL();
    int Sra(RegisterU8& reg);
    int SraHL();
    int Srl(RegisterU8& reg);
    int SrlHL();
    int Swap(RegisterU8& reg);
    int SwapHL();

    // Bit Operations
    int Bit(RegisterU8& reg, u8 bit);
    int BitHL(u8 bit);
    int Set(RegisterU8& reg, u8 bit);
    int SetHL(u8 bit);
    int Res(RegisterU8& reg, u8 bit);
    int ResHL(u8 bit);

    // Jump instructions
    int Jpnn(u16 value);
    int Jpcc(u8 cc, u16 value);
    int Jre(s8 value);
    int Jrecc(u8 cc, s8 value);
    int JpHL();

    // 8-bit Arithmetic
    int Add(RegisterU8& reg);                       // 10000xxx (r) -> 1
    int Add(u8 value);                              // 11000110 + xxxxxxx (n) -> 2
    int AddHL();                                    // 10000110 -> 2
    int Adc(RegisterU8& reg);                       // 10001xxx (r) -> 1
    int Adc(u8 value);                              // 11001110 + xxxxxxxx (n) -> 2
    int AdcHL();                                    // 10001110 -> 2
    int Sub(RegisterU8& reg);                       // 10010xxx (r) -> 1
    int Sub(u8 value);                              // 11010110 + xxxxxxxx (n) -> 2
    int SubHL();                                    // 10010110 -> 2
    int Sbc(RegisterU8& reg);                       // 10011xxx (r) -> 1
    int Sbc(u8 value);                              // 11011110 + xxxxxxx (n) -> 2
    int SbcHL();                                    // 10011110 -> 2

    // 8-bit Logical Operators
    int And(RegisterU8& reg);                       // 10100xxx (r) -> 1
    int And(u8 value);                              // 11100110 + xxxxxxxx (n) -> 2
    int AndHL();                                    // 10100110 -> 2
    int Or(RegisterU8& reg);                        // 10110xxx (r) -> 1
    int Or(u8 value);                               // 11110110 + xxxxxxxx (n) -> 2
    int OrHL();                                     // 10110110 -> 2
    int Xor(RegisterU8& reg);                       // 10101xxx (r) -> 1
    int Xor(u8 value);                              // 11101110 + xxxxxxxx (n) -> 2
    int XorHL();                                    // 10101110 -> 2
    int Cmp(RegisterU8& reg);                       // 10111xxx (r) -> 1
    int Cmp(u8 value);                              // 11111110 + xxxxxxxx (n) -> 2
    int CmpHL();                                    // 10111110 -> 2
    int Inc(RegisterU8& reg);                       // 00xxx100 (r) -> 1
    int IncHL();                                    // 00110100 -> 3
    int Dec(RegisterU8& reg);                       // 00xxx101 (r) -> 1
    int DecHL();                                    // 00110101 -> 3
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

