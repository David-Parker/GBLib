#pragma once
#include "Address.h"
#include "GlobalDefinitions.h"
#include "Memory.h"
#include "RegisterU8.h"
#include "RegisterU16.h"
#include <functional>
#include <utility>

#define CLOCK_HZ 4194304
#define CLOCK_NS_PER_CYCLE 238
#define CLOCK_CYCLES_PER_MACHINE_CYCLE 4

// Instruction set reference https://gekkio.fi/files/gb-docs/gbctr.pdf and https://meganesulli.com/generate-gb-opcodes/ and https://gbdev.io/pandocs/Specifications.html
class Cpu
{
private:
    // Memory
    Memory* pMemory;

    // Registers
    RegisterU16 BC; // 00
    RegisterU16 DE; // 01
    RegisterU16 HL; // 10
    RegisterU16 SP; // 11 Stack Pointer
    RegisterU16 AF; // Accumulator & Flags
    RegisterU16 PC; // Program Counter

    RegisterU8 B; // 000
    RegisterU8 C; // 001
    RegisterU8 D; // 010
    RegisterU8 E; // 011
    RegisterU8 H; // 100
    RegisterU8 L; // 101
    RegisterU8 A; // 111 Accumulator
    RegisterU8 F; // Flags

	// Interrupt flags
	u8 IME; // Interrupt Master Enable
	u8 IE; // Interrupt Enable
	u8 IF; // Interrupt Flag

	typedef std::function<int()> instruction_t;

	instruction_t opcodes[256];
	instruction_t opcodes_16[256];

	char* opcode_strings[256];
	char* opcode_strings_16[256];

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
	u8 ReadByte();
	u16 ReadTwoBytes();

#pragma region Instructions
    // 8-bit Transfer and I/O                       // opcode   (operands) -> cycles
    int Nop();                                      // 00000000 -> 0
	int Stop();										// 00000001 -> 1
    int Ld(RegisterU8& dest, RegisterU8& src);      // 01xxxyyy (r, r') -> 1
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
    int Rlc(RegisterU8& reg);                       // 11001011 + 00000xxx (r) -> 2
    int RlcHL();                                    // 11001011 + 00000110 -> 4
    int Rl(RegisterU8& reg);                        // 11001011 + 00010xxx (r) -> 2
    int RlHL();                                     // 11001011 + 00010110 -> 4
    int Rrc(RegisterU8& reg);                       // 11001011 + 00001xxx (r) -> 2
    int RrcHL();                                    // 11001011 + 00001110 -> 4
    int Rr(RegisterU8& reg);                        // 11001011 + 00011xxx (r) -> 2
    int RrHL();                                     // 11011011 + 00011110 -> 4
    int Sla(RegisterU8& reg);                       // 11001011 + 00100xxx (r) -> 2
    int SlaHL();                                    // 11011011 + 00100110 -> 4
    int Sra(RegisterU8& reg);                       // 11001011 + 00101xxx (r) -> 2
    int SraHL();                                    // 11001011 + 00101110 -> 4
    int Srl(RegisterU8& reg);                       // 11001011 + 00111xxx (r) -> 2
    int SrlHL();                                    // 11001011 + 00111110 -> 4
    int Swap(RegisterU8& reg);                      // 11001011 + 00110xxx (r) -> 2
    int SwapHL();                                   // 11001011 + 00110110 -> 4

    // Bit Operations
    int Bit(RegisterU8& reg, u8 bit);               // 11001011 + 01xxxyyy (b, r) -> 2
    int BitHL(u8 bit);                              // 11001011 + 01xxx110 (b) -> 3
    int Set(RegisterU8& reg, u8 bit);               // 11001011 + 11xxxyyy (b, r) -> 2
    int SetHL(u8 bit);                              // 11001011 + 11xxx110 (b) -> 4
    int Res(RegisterU8& reg, u8 bit);               // 11001011 + 10xxxyyy (b, r) -> 2
    int ResHL(u8 bit);                              // 11001011 + 10xxx110 (b) -> 4

    // Jump instructions
    int Jpnn(u16 value);                            // 11000011 + xxxxxxxx (n) + xxxxxxxx (n) -> 4
    int Jpcc(u8 cc, u16 value);                     // 110xx010 (cc) + xxxxxxxx (n) + xxxxxxxx (n) -> 4/3
    int Jre(s8 value);                              // 00011000 + xxxxxxxx (e) -> 3
    int Jrecc(u8 cc, s8 value);                     // 001xx000 (cc) + xxxxxxxx (e) -> 3/2
    int JpHL();                                     // 11101001 -> 1

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
    // On startup, stack pointer is set to 0xFFFE and the PC starts at 0x0 which is the boot ROM.
    Cpu(Memory* pMemory) : 
		SP(0xFFFE), 
		PC(0x000), 
		pMemory(pMemory),
        A((u8*)&AF + 1), 
		B((u8*)&BC + 1), 
		C((u8*)&BC), 
		D((u8*)&DE + 1),
        E((u8*)&DE), 
		F((u8*)&AF), 
		H((u8*)&HL + 1), 
		L((u8*)&HL),
		IME(0x00), 
		IF(0xe0), 
		IE(0x00)
	{
		// Set all opcodes initially to an error state. This makes debugging missing opcodes easier.
		for (int i = 0; i < 256; ++i)
		{
			opcode_strings[i] = "XXX";
			opcodes[i] = []() { return -1; };

			opcode_strings_16[i] = "XXX";
			opcodes_16[i] = []() { return -1; };
		}

		// 8-bit Opcodes
		opcodes[0x00] = [this]() { return Nop(); };

		opcodes[0x0E] = [this]() { return Ld(C, ReadByte()); };
		opcodes[0x1E] = [this]() { return Ld(E, ReadByte()); };
		opcodes[0x2E] = [this]() { return Ld(L, ReadByte()); };
		opcodes[0x3E] = [this]() { return Ld(A, ReadByte()); };
		opcodes[0x4E] = [this]() { return LdrHL(C); };
		opcodes[0x5E] = [this]() { return LdrHL(E); };
		opcodes[0x6E] = [this]() { return LdrHL(L); };
		opcodes[0x7E] = [this]() { return LdrHL(A); };

		opcodes[0x01] = [this]() { return Ldnn(BC, ReadTwoBytes()); };
		opcodes[0x11] = [this]() { return Ldnn(DE, ReadTwoBytes()); };
		opcodes[0x21] = [this]() { return Ldnn(HL, ReadTwoBytes()); };
		opcodes[0x31] = [this]() { return Ldnn(SP, ReadTwoBytes()); };

		opcodes[0x02] = [this]() { return LdBCA(); };
		opcodes[0x12] = [this]() { return LdDEA(); };
		opcodes[0x22] = [this]() { return LdHLIA(); };
		opcodes[0x32] = [this]() { return LdHLDA(); };

		opcodes[0xA8] = [this]() { return Xor(B); };
		opcodes[0xA9] = [this]() { return Xor(C); };
		opcodes[0xAA] = [this]() { return Xor(D); };
		opcodes[0xAB] = [this]() { return Xor(E); };
		opcodes[0xAC] = [this]() { return Xor(H); };
		opcodes[0xAD] = [this]() { return Xor(L); };
		opcodes[0xAE] = [this]() { return XorHL(); };
		opcodes[0xAF] = [this]() { return Xor(A); };

		opcodes[0xC3] = [this]() { return Jpnn(ReadTwoBytes()); };

		opcodes[0x40] = [this]() { return Ld(B, B); };
		opcodes[0x41] = [this]() { return Ld(B, C); };
		opcodes[0x42] = [this]() { return Ld(B, D); };
		opcodes[0x43] = [this]() { return Ld(B, E); };
		opcodes[0x44] = [this]() { return Ld(B, H); };
		opcodes[0x45] = [this]() { return Ld(B, L); };

		opcodes[0xE0] = [this]() { return LdnA(ReadByte()); };

		// 16-bit Opcodes
		opcodes_16[0x40] = [this]() { return Bit(B, 0); };
		opcodes_16[0x41] = [this]() { return Bit(C, 0); };
		opcodes_16[0x42] = [this]() { return Bit(D, 0); };
		opcodes_16[0x43] = [this]() { return Bit(E, 0); };
		opcodes_16[0x44] = [this]() { return Bit(H, 0); };
		opcodes_16[0x45] = [this]() { return Bit(L, 0); };
		opcodes_16[0x46] = [this]() { return BitHL(0); };
		opcodes_16[0x47] = [this]() { return Bit(A, 0); };
		opcodes_16[0x48] = [this]() { return Bit(B, 1); };
		opcodes_16[0x49] = [this]() { return Bit(C, 1); };
		opcodes_16[0x4A] = [this]() { return Bit(D, 1); };
		opcodes_16[0x4B] = [this]() { return Bit(E, 1); };
		opcodes_16[0x4C] = [this]() { return Bit(H, 1); };
		opcodes_16[0x4D] = [this]() { return Bit(L, 1); };
		opcodes_16[0x4E] = [this]() { return BitHL(1); };
		opcodes_16[0x4F] = [this]() { return Bit(A, 1); };
		opcodes_16[0x50] = [this]() { return Bit(B, 2); };
		opcodes_16[0x51] = [this]() { return Bit(C, 2); };
		opcodes_16[0x52] = [this]() { return Bit(D, 2); };
		opcodes_16[0x53] = [this]() { return Bit(E, 2); };
		opcodes_16[0x54] = [this]() { return Bit(H, 2); };
		opcodes_16[0x55] = [this]() { return Bit(L, 2); };
		opcodes_16[0x56] = [this]() { return BitHL(2); };
		opcodes_16[0x57] = [this]() { return Bit(A, 2); };
		opcodes_16[0x58] = [this]() { return Bit(B, 3); };
		opcodes_16[0x59] = [this]() { return Bit(C, 3); };
		opcodes_16[0x5A] = [this]() { return Bit(D, 3); };
		opcodes_16[0x5B] = [this]() { return Bit(E, 3); };
		opcodes_16[0x5C] = [this]() { return Bit(H, 3); };
		opcodes_16[0x5D] = [this]() { return Bit(L, 3); };
		opcodes_16[0x5E] = [this]() { return BitHL(3); };
		opcodes_16[0x5F] = [this]() { return Bit(A, 3); };
		opcodes_16[0x60] = [this]() { return Bit(B, 4); };
		opcodes_16[0x61] = [this]() { return Bit(C, 4); };
		opcodes_16[0x62] = [this]() { return Bit(D, 4); };
		opcodes_16[0x63] = [this]() { return Bit(E, 4); };
		opcodes_16[0x64] = [this]() { return Bit(H, 4); };
		opcodes_16[0x65] = [this]() { return Bit(L, 4); };
		opcodes_16[0x66] = [this]() { return BitHL(4); };
		opcodes_16[0x67] = [this]() { return Bit(A, 4); };
		opcodes_16[0x68] = [this]() { return Bit(B, 5); };
		opcodes_16[0x69] = [this]() { return Bit(C, 5); };
		opcodes_16[0x6A] = [this]() { return Bit(D, 5); };
		opcodes_16[0x6B] = [this]() { return Bit(E, 5); };
		opcodes_16[0x6C] = [this]() { return Bit(H, 5); };
		opcodes_16[0x6D] = [this]() { return Bit(L, 5); };
		opcodes_16[0x6E] = [this]() { return BitHL(5); };
		opcodes_16[0x6F] = [this]() { return Bit(A, 5); };
		opcodes_16[0x70] = [this]() { return Bit(B, 6); };
		opcodes_16[0x71] = [this]() { return Bit(C, 6); };
		opcodes_16[0x72] = [this]() { return Bit(D, 6); };
		opcodes_16[0x73] = [this]() { return Bit(E, 6); };
		opcodes_16[0x74] = [this]() { return Bit(H, 6); };
		opcodes_16[0x75] = [this]() { return Bit(L, 6); };
		opcodes_16[0x76] = [this]() { return BitHL(6); };
		opcodes_16[0x77] = [this]() { return Bit(A, 6); };
		opcodes_16[0x78] = [this]() { return Bit(B, 7); };
		opcodes_16[0x79] = [this]() { return Bit(C, 7); };
		opcodes_16[0x7A] = [this]() { return Bit(D, 7); };
		opcodes_16[0x7B] = [this]() { return Bit(E, 7); };
		opcodes_16[0x7C] = [this]() { return Bit(H, 7); };
		opcodes_16[0x7D] = [this]() { return Bit(L, 7); };
		opcodes_16[0x7E] = [this]() { return BitHL(7); };
		opcodes_16[0x7F] = [this]() { return Bit(A, 7); };

		// 8-Bit Opcode Strings
		opcode_strings[0x00] = "[this]() { return Nop(); };";

		opcode_strings[0x0E] = "[this]() { return Ld(C, ReadByte()); };";
		opcode_strings[0x1E] = "[this]() { return Ld(E, ReadByte()); };";
		opcode_strings[0x2E] = "[this]() { return Ld(L, ReadByte()); };";
		opcode_strings[0x3E] = "[this]() { return Ld(A, ReadByte()); };";

		opcode_strings[0x4E] = "[this]() { return LdrHL(C); };";
		opcode_strings[0x5E] = "[this]() { return LdrHL(E); };";
		opcode_strings[0x6E] = "[this]() { return LdrHL(L); };";
		opcode_strings[0x7E] = "[this]() { return LdrHL(A); };";

		opcode_strings[0x01] = "[this]() { return Ldnn(BC, ReadTwoBytes()); };";
		opcode_strings[0x11] = "[this]() { return Ldnn(DE, ReadTwoBytes()); };";
		opcode_strings[0x21] = "[this]() { return Ldnn(HL, ReadTwoBytes()); };";
		opcode_strings[0x31] = "[this]() { return Ldnn(SP, ReadTwoBytes()); };";

		opcode_strings[0x02] = "[this]() { return LdBCA(); };";
		opcode_strings[0x12] = "[this]() { return LdDEA(); };";
		opcode_strings[0x22] = "[this]() { return LdHLIA(); };";
		opcode_strings[0x32] = "[this]() { return LdHLDA(); };";

		opcode_strings[0xA8] = "[this]() { return Xor(B); };";
		opcode_strings[0xA9] = "[this]() { return Xor(C); };";
		opcode_strings[0xAA] = "[this]() { return Xor(D); };";
		opcode_strings[0xAB] = "[this]() { return Xor(E); };";
		opcode_strings[0xAC] = "[this]() { return Xor(H); };";
		opcode_strings[0xAD] = "[this]() { return Xor(L); };";
		opcode_strings[0xAE] = "[this]() { return XorHL(); };";
		opcode_strings[0xAF] = "[this]() { return Xor(A); };";

		opcode_strings[0xC3] = "[this]() { return Jpnn(ReadTwoBytes()); };";

		opcode_strings[0x40] = "[this]() { return Ld(B,B); };";
		opcode_strings[0x41] = "[this]() { return Ld(B,C); };";
		opcode_strings[0x42] = "[this]() { return Ld(B,D); };";
		opcode_strings[0x43] = "[this]() { return Ld(B,E); };";
		opcode_strings[0x44] = "[this]() { return Ld(B,H); };";
		opcode_strings[0x45] = "[this]() { return Ld(B,L); };";

		opcode_strings[0xE0] = "[this]() { return LdnA(ReadByte()); };";

		// 16-bit Opcode Strings
		opcode_strings_16[0x40] = "[this]() { return Bit(B, 0); };";
		opcode_strings_16[0x41] = "[this]() { return Bit(C, 0); };";
		opcode_strings_16[0x42] = "[this]() { return Bit(D, 0); };";
		opcode_strings_16[0x43] = "[this]() { return Bit(E, 0); };";
		opcode_strings_16[0x44] = "[this]() { return Bit(H, 0); };";
		opcode_strings_16[0x45] = "[this]() { return Bit(L, 0); };";
		opcode_strings_16[0x46] = "[this]() { return BitHL(0); };";
		opcode_strings_16[0x47] = "[this]() { return Bit(A, 0); };";
		opcode_strings_16[0x48] = "[this]() { return Bit(B, 1); };";
		opcode_strings_16[0x49] = "[this]() { return Bit(C, 1); };";
		opcode_strings_16[0x4A] = "[this]() { return Bit(D, 1); };";
		opcode_strings_16[0x4B] = "[this]() { return Bit(E, 1); };";
		opcode_strings_16[0x4C] = "[this]() { return Bit(H, 1); };";
		opcode_strings_16[0x4D] = "[this]() { return Bit(L, 1); };";
		opcode_strings_16[0x4E] = "[this]() { return BitHL(1); };";
		opcode_strings_16[0x4F] = "[this]() { return Bit(A, 1); };";
		opcode_strings_16[0x50] = "[this]() { return Bit(B, 2); };";
		opcode_strings_16[0x51] = "[this]() { return Bit(C, 2); };";
		opcode_strings_16[0x52] = "[this]() { return Bit(D, 2); };";
		opcode_strings_16[0x53] = "[this]() { return Bit(E, 2); };";
		opcode_strings_16[0x54] = "[this]() { return Bit(H, 2); };";
		opcode_strings_16[0x55] = "[this]() { return Bit(L, 2); };";
		opcode_strings_16[0x56] = "[this]() { return BitHL(2); };";
		opcode_strings_16[0x57] = "[this]() { return Bit(A, 2); };";
		opcode_strings_16[0x58] = "[this]() { return Bit(B, 3); };";
		opcode_strings_16[0x59] = "[this]() { return Bit(C, 3); };";
		opcode_strings_16[0x5A] = "[this]() { return Bit(D, 3); };";
		opcode_strings_16[0x5B] = "[this]() { return Bit(E, 3); };";
		opcode_strings_16[0x5C] = "[this]() { return Bit(H, 3); };";
		opcode_strings_16[0x5D] = "[this]() { return Bit(L, 3); };";
		opcode_strings_16[0x5E] = "[this]() { return BitHL(3); };";
		opcode_strings_16[0x5F] = "[this]() { return Bit(A, 3); };";
		opcode_strings_16[0x60] = "[this]() { return Bit(B, 4); };";
		opcode_strings_16[0x61] = "[this]() { return Bit(C, 4); };";
		opcode_strings_16[0x62] = "[this]() { return Bit(D, 4); };";
		opcode_strings_16[0x63] = "[this]() { return Bit(E, 4); };";
		opcode_strings_16[0x64] = "[this]() { return Bit(H, 4); };";
		opcode_strings_16[0x65] = "[this]() { return Bit(L, 4); };";
		opcode_strings_16[0x66] = "[this]() { return BitHL(4); };";
		opcode_strings_16[0x67] = "[this]() { return Bit(A, 4); };";
		opcode_strings_16[0x68] = "[this]() { return Bit(B, 5); };";
		opcode_strings_16[0x69] = "[this]() { return Bit(C, 5); };";
		opcode_strings_16[0x6A] = "[this]() { return Bit(D, 5); };";
		opcode_strings_16[0x6B] = "[this]() { return Bit(E, 5); };";
		opcode_strings_16[0x6C] = "[this]() { return Bit(H, 5); };";
		opcode_strings_16[0x6D] = "[this]() { return Bit(L, 5); };";
		opcode_strings_16[0x6E] = "[this]() { return BitHL(5); };";
		opcode_strings_16[0x6F] = "[this]() { return Bit(A, 5); };";
		opcode_strings_16[0x70] = "[this]() { return Bit(B, 6); };";
		opcode_strings_16[0x71] = "[this]() { return Bit(C, 6); };";
		opcode_strings_16[0x72] = "[this]() { return Bit(D, 6); };";
		opcode_strings_16[0x73] = "[this]() { return Bit(E, 6); };";
		opcode_strings_16[0x74] = "[this]() { return Bit(H, 6); };";
		opcode_strings_16[0x75] = "[this]() { return Bit(L, 6); };";
		opcode_strings_16[0x76] = "[this]() { return BitHL(6); };";
		opcode_strings_16[0x77] = "[this]() { return Bit(A, 6); };";
		opcode_strings_16[0x78] = "[this]() { return Bit(B, 7); };";
		opcode_strings_16[0x79] = "[this]() { return Bit(C, 7); };";
		opcode_strings_16[0x7A] = "[this]() { return Bit(D, 7); };";
		opcode_strings_16[0x7B] = "[this]() { return Bit(E, 7); };";
		opcode_strings_16[0x7C] = "[this]() { return Bit(H, 7); };";
		opcode_strings_16[0x7D] = "[this]() { return Bit(L, 7); };";
		opcode_strings_16[0x7E] = "[this]() { return BitHL(7); };";
		opcode_strings_16[0x7F] = "[this]() { return Bit(A, 7); };";
	}

    ~Cpu();

    // Executes and returns the number of machine cycles to perform the next instruction
    int Tick();
};