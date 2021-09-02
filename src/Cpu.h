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

#define F_NZ 0x00
#define F_Z  0x01
#define F_NC 0x02
#define F_C  0x03

// Official GameBoy Programming Manual v1.1 by Nintendo: https://www.manualslib.com/manual/1627081/Nintendo-Game-Boy.html?page=7#manual
// Opcode table: https://meganesulli.com/generate-gb-opcodes/
// Incomplete but useful docs: https://gekkio.fi/files/gb-docs/gbctr.pdf
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
	bool FlagMatchesCC(u8 cc);
	u8 ReadByte();
	u16 ReadTwoBytes();

#pragma region Instructions
	// Cpu Control									// opcode   (operands) -> M cycles
	int Daa();										// 00100111 -> 1
	int Cpl();										// 00101111 -> 1
	int Nop();                                      // 00000000 -> 1
	int Ccf();										// 00111111 -> 1
	int Scf();										// 00110111 -> 1
	int Di();										// 11110011 -> 1
	int Ei();										// 11111011 -> 1
	int Halt(); // TODO this is not implemented		// 01110110 -> 1
	int Stop(); // TODO this is not implemented 	// 00010000 -> 1

    // 8-bit Transfer and I/O                       
    int Ld(RegisterU8& dest, RegisterU8& src);      // 01xxxyyy (r, r') -> 1
    int Ld(RegisterU8& dest, u8 value);             // 00xxx110 (r) + xxxxxxxx (n) -> 2
    int LdrHL(RegisterU8& src);                     // 01xxx110 (r) -> 2
    int LdHLr(RegisterU8& src);                     // 01110xxx (r) -> 2
    int LdHLn(u8 value);                            // 00110110 + xxxxxxxx (n) -> 3
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
    int Bit(u8 bit, RegisterU8& reg);               // 11001011 + 01xxxyyy (b, r) -> 2
    int BitHL(u8 bit);                              // 11001011 + 01xxx110 (b) -> 3
    int Set(u8 bit, RegisterU8& reg);               // 11001011 + 11xxxyyy (b, r) -> 2
    int SetHL(u8 bit);                              // 11001011 + 11xxx110 (b) -> 4
    int Res(u8 bit, RegisterU8& reg);               // 11001011 + 10xxxyyy (b, r) -> 2
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

	// Call and Return
	int Callnn(u16 value);							// 11001101 + xxxxxxxx (n) + xxxxxxxx (n) -> 6
	int Callcc(u8 cc, u16 value);					// 110xx100 (cc) + xxxxxxxx (n) + xxxxxxxx (n) -> 6/3
	int Ret();										// 11001001 -> 4
	int RetI();										// 11011001 -> 4
	int Retcc(u8 cc);								// 110xx000 (cc) -> 5/2
	int Rst(u8 t);									// 11xxx111 (t) -> 4

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
		opcodes[0x10] = [this]() { return Stop(); };
		opcodes[0x20] = [this]() { return Jrecc(F_NZ, ReadByte()); };
		opcodes[0x30] = [this]() { return Jrecc(F_NC, ReadByte()); };

		opcodes[0x01] = [this]() { return Ldnn(BC, ReadTwoBytes()); };
		opcodes[0x11] = [this]() { return Ldnn(DE, ReadTwoBytes()); };
		opcodes[0x21] = [this]() { return Ldnn(HL, ReadTwoBytes()); };
		opcodes[0x31] = [this]() { return Ldnn(SP, ReadTwoBytes()); };

		opcodes[0x02] = [this]() { return LdBCA(); };
		opcodes[0x12] = [this]() { return LdDEA(); };
		opcodes[0x22] = [this]() { return LdHLIA(); };
		opcodes[0x32] = [this]() { return LdHLDA(); };

		opcodes[0x03] = [this]() { return Inc(BC); };
		opcodes[0x13] = [this]() { return Inc(DE); };
		opcodes[0x23] = [this]() { return Inc(HL); };
		opcodes[0x33] = [this]() { return Inc(SP); };

		opcodes[0x04] = [this]() { return Inc(B); };
		opcodes[0x14] = [this]() { return Inc(D); };
		opcodes[0x24] = [this]() { return Inc(H); };
		opcodes[0x34] = [this]() { return IncHL(); };

		opcodes[0x05] = [this]() { return Dec(B); };
		opcodes[0x15] = [this]() { return Dec(D); };
		opcodes[0x25] = [this]() { return Dec(H); };
		opcodes[0x35] = [this]() { return DecHL(); };

		opcodes[0x06] = [this]() { return Ld(B, ReadByte()); };
		opcodes[0x16] = [this]() { return Ld(D, ReadByte()); };
		opcodes[0x26] = [this]() { return Ld(H, ReadByte()); };
		opcodes[0x36] = [this]() { return LdHLn(ReadByte()); };

		opcodes[0x07] = [this]() { return RlcA(); };
		opcodes[0x17] = [this]() { return RlA(); };
		opcodes[0x27] = [this]() { return Daa(); };
		opcodes[0x37] = [this]() { return Scf(); };

		opcodes[0x08] = [this]() { return LdnnSP(ReadTwoBytes()); };
		opcodes[0x18] = [this]() { return Jre(ReadByte()); };
		opcodes[0x28] = [this]() { return Jrecc(F_Z, ReadByte()); };
		opcodes[0x38] = [this]() { return Jrecc(F_C, ReadByte()); };

		opcodes[0x09] = [this]() { return AddHL(BC); };
		opcodes[0x19] = [this]() { return AddHL(DE); };
		opcodes[0x29] = [this]() { return AddHL(HL); };
		opcodes[0x39] = [this]() { return AddHL(SP); };

		opcodes[0x0A] = [this]() { return LdABC(); };
		opcodes[0x1A] = [this]() { return LdADE(); };
		opcodes[0x2A] = [this]() { return LdAHLI(); };
		opcodes[0x3A] = [this]() { return LdAHLD(); };

		opcodes[0x0B] = [this]() { return Dec(BC); };
		opcodes[0x1B] = [this]() { return Dec(DE); };
		opcodes[0x2B] = [this]() { return Dec(HL); };
		opcodes[0x3B] = [this]() { return Dec(SP); };

		opcodes[0x0C] = [this]() { return Inc(C); };
		opcodes[0x1C] = [this]() { return Inc(E); };
		opcodes[0x2C] = [this]() { return Inc(L); };
		opcodes[0x3C] = [this]() { return Inc(A); };

		opcodes[0x0D] = [this]() { return Dec(C); };
		opcodes[0x1D] = [this]() { return Dec(E); };
		opcodes[0x2D] = [this]() { return Dec(L); };
		opcodes[0x3D] = [this]() { return Dec(A); };

		opcodes[0x0E] = [this]() { return Ld(C, ReadByte()); };
		opcodes[0x1E] = [this]() { return Ld(E, ReadByte()); };
		opcodes[0x2E] = [this]() { return Ld(L, ReadByte()); };
		opcodes[0x3E] = [this]() { return Ld(A, ReadByte()); };

		opcodes[0x0F] = [this]() { return RrcA(); };
		opcodes[0x1F] = [this]() { return RrA(); };
		opcodes[0x2F] = [this]() { return Cpl(); };
		opcodes[0x3F] = [this]() { return Ccf(); };

		opcodes[0x40] = [this]() { return Ld(B, B); };
		opcodes[0x41] = [this]() { return Ld(B, C); };
		opcodes[0x42] = [this]() { return Ld(B, D); };
		opcodes[0x43] = [this]() { return Ld(B, E); };
		opcodes[0x44] = [this]() { return Ld(B, H); };
		opcodes[0x45] = [this]() { return Ld(B, L); };
		opcodes[0x46] = [this]() { return LdrHL(B); };
		opcodes[0x47] = [this]() { return Ld(B, A); };
		opcodes[0x48] = [this]() { return Ld(C, B); };
		opcodes[0x49] = [this]() { return Ld(C, C); };
		opcodes[0x4A] = [this]() { return Ld(C, D); };
		opcodes[0x4B] = [this]() { return Ld(C, E); };
		opcodes[0x4C] = [this]() { return Ld(C, H); };
		opcodes[0x4D] = [this]() { return Ld(C, L); };
		opcodes[0x4E] = [this]() { return LdrHL(C); };
		opcodes[0x4F] = [this]() { return Ld(C, A); };
		opcodes[0x50] = [this]() { return Ld(D, B); };
		opcodes[0x51] = [this]() { return Ld(D, C); };
		opcodes[0x52] = [this]() { return Ld(D, D); };
		opcodes[0x53] = [this]() { return Ld(D, E); };
		opcodes[0x54] = [this]() { return Ld(D, H); };
		opcodes[0x55] = [this]() { return Ld(D, L); };
		opcodes[0x56] = [this]() { return LdrHL(D); };
		opcodes[0x57] = [this]() { return Ld(D, A); };
		opcodes[0x58] = [this]() { return Ld(E, B); };
		opcodes[0x59] = [this]() { return Ld(E, C); };
		opcodes[0x5A] = [this]() { return Ld(E, D); };
		opcodes[0x5B] = [this]() { return Ld(E, E); };
		opcodes[0x5C] = [this]() { return Ld(E, H); };
		opcodes[0x5D] = [this]() { return Ld(E, L); };
		opcodes[0x5E] = [this]() { return LdrHL(E); };
		opcodes[0x5F] = [this]() { return Ld(E, A); };
		opcodes[0x60] = [this]() { return Ld(H, B); };
		opcodes[0x61] = [this]() { return Ld(H, C); };
		opcodes[0x62] = [this]() { return Ld(H, D); };
		opcodes[0x63] = [this]() { return Ld(H, E); };
		opcodes[0x64] = [this]() { return Ld(H, H); };
		opcodes[0x65] = [this]() { return Ld(H, L); };
		opcodes[0x66] = [this]() { return LdrHL(H); };
		opcodes[0x67] = [this]() { return Ld(H, A); };
		opcodes[0x68] = [this]() { return Ld(L, B); };
		opcodes[0x69] = [this]() { return Ld(L, C); };
		opcodes[0x6A] = [this]() { return Ld(L, D); };
		opcodes[0x6B] = [this]() { return Ld(L, E); };
		opcodes[0x6C] = [this]() { return Ld(L, H); };
		opcodes[0x6D] = [this]() { return Ld(L, L); };
		opcodes[0x6E] = [this]() { return LdrHL(L); };
		opcodes[0x6F] = [this]() { return Ld(L, A); };
		opcodes[0x70] = [this]() { return LdHLr(B); };
		opcodes[0x71] = [this]() { return LdHLr(C); };
		opcodes[0x72] = [this]() { return LdHLr(D); };
		opcodes[0x73] = [this]() { return LdHLr(E); };
		opcodes[0x74] = [this]() { return LdHLr(H); };
		opcodes[0x75] = [this]() { return LdHLr(L); };
		opcodes[0x76] = [this]() { return Halt(); };
		opcodes[0x77] = [this]() { return LdHLr(A); };
		opcodes[0x78] = [this]() { return Ld(A, B); };
		opcodes[0x79] = [this]() { return Ld(A, C); };
		opcodes[0x7A] = [this]() { return Ld(A, D); };
		opcodes[0x7B] = [this]() { return Ld(A, E); };
		opcodes[0x7C] = [this]() { return Ld(A, H); };
		opcodes[0x7D] = [this]() { return Ld(A, L); };
		opcodes[0x7E] = [this]() { return LdrHL(A); };
		opcodes[0x7F] = [this]() { return Ld(A, A); };

		opcodes[0x80] = [this]() { return Add(B); };
		opcodes[0x81] = [this]() { return Add(C); };
		opcodes[0x82] = [this]() { return Add(D); };
		opcodes[0x83] = [this]() { return Add(E); };
		opcodes[0x84] = [this]() { return Add(H); };
		opcodes[0x85] = [this]() { return Add(L); };
		opcodes[0x86] = [this]() { return AddHL(); };
		opcodes[0x87] = [this]() { return Add(A); };
		opcodes[0x88] = [this]() { return Adc(B); };
		opcodes[0x89] = [this]() { return Adc(C); };
		opcodes[0x8A] = [this]() { return Adc(D); };
		opcodes[0x8B] = [this]() { return Adc(E); };
		opcodes[0x8C] = [this]() { return Adc(H); };
		opcodes[0x8D] = [this]() { return Adc(L); };
		opcodes[0x8E] = [this]() { return AdcHL(); };
		opcodes[0x8F] = [this]() { return Adc(A); };
		opcodes[0x90] = [this]() { return Sub(B); };
		opcodes[0x91] = [this]() { return Sub(C); };
		opcodes[0x92] = [this]() { return Sub(D); };
		opcodes[0x93] = [this]() { return Sub(E); };
		opcodes[0x94] = [this]() { return Sub(H); };
		opcodes[0x95] = [this]() { return Sub(L); };
		opcodes[0x96] = [this]() { return SubHL(); };
		opcodes[0x97] = [this]() { return Sub(A); };
		opcodes[0x98] = [this]() { return Sbc(B); };
		opcodes[0x99] = [this]() { return Sbc(C); };
		opcodes[0x9A] = [this]() { return Sbc(D); };
		opcodes[0x9B] = [this]() { return Sbc(E); };
		opcodes[0x9C] = [this]() { return Sbc(H); };
		opcodes[0x9D] = [this]() { return Sbc(L); };
		opcodes[0x9E] = [this]() { return SbcHL(); };
		opcodes[0x9F] = [this]() { return Sbc(A); };
		opcodes[0xA0] = [this]() { return And(B); };
		opcodes[0xA1] = [this]() { return And(C); };
		opcodes[0xA2] = [this]() { return And(D); };
		opcodes[0xA3] = [this]() { return And(E); };
		opcodes[0xA4] = [this]() { return And(H); };
		opcodes[0xA5] = [this]() { return And(L); };
		opcodes[0xA6] = [this]() { return AndHL(); };
		opcodes[0xA7] = [this]() { return And(A); };
		opcodes[0xA8] = [this]() { return Xor(B); };
		opcodes[0xA9] = [this]() { return Xor(C); };
		opcodes[0xAA] = [this]() { return Xor(D); };
		opcodes[0xAB] = [this]() { return Xor(E); };
		opcodes[0xAC] = [this]() { return Xor(H); };
		opcodes[0xAD] = [this]() { return Xor(L); };
		opcodes[0xAE] = [this]() { return XorHL(); };
		opcodes[0xAF] = [this]() { return Xor(A); };
		opcodes[0xB0] = [this]() { return Or(B); };
		opcodes[0xB1] = [this]() { return Or(C); };
		opcodes[0xB2] = [this]() { return Or(D); };
		opcodes[0xB3] = [this]() { return Or(E); };
		opcodes[0xB4] = [this]() { return Or(H); };
		opcodes[0xB5] = [this]() { return Or(L); };
		opcodes[0xB6] = [this]() { return OrHL(); };
		opcodes[0xB7] = [this]() { return Or(A); };
		opcodes[0xB8] = [this]() { return Cmp(B); };
		opcodes[0xB9] = [this]() { return Cmp(C); };
		opcodes[0xBA] = [this]() { return Cmp(D); };
		opcodes[0xBB] = [this]() { return Cmp(E); };
		opcodes[0xBC] = [this]() { return Cmp(H); };
		opcodes[0xBD] = [this]() { return Cmp(L); };
		opcodes[0xBE] = [this]() { return CmpHL(); };
		opcodes[0xBF] = [this]() { return Cmp(A); };

		opcodes[0xC0] = [this]() { return Retcc(F_NZ); };
		opcodes[0xD0] = [this]() { return Retcc(F_NC); };
		opcodes[0xE0] = [this]() { return LdnA(ReadByte()); };
		opcodes[0xF0] = [this]() { return LdAn(ReadByte()); };

		opcodes[0xC1] = [this]() { return Pop(BC); };
		opcodes[0xD1] = [this]() { return Pop(DE); };
		opcodes[0xE1] = [this]() { return Pop(HL); };
		opcodes[0xF1] = [this]() { return Pop(SP); };

		opcodes[0xC2] = [this]() { return Jpcc(F_NZ, ReadTwoBytes()); };
		opcodes[0xD2] = [this]() { return Jpcc(F_NC, ReadTwoBytes()); };
		opcodes[0xE2] = [this]() { return LdCA(); };
		opcodes[0xF2] = [this]() { return LdAC(); };

		opcodes[0xC3] = [this]() { return Jpnn(ReadTwoBytes()); };
		opcodes[0xF3] = [this]() { return Di(); };

		opcodes[0xC4] = [this]() { return Callcc(F_NZ, ReadTwoBytes()); };
		opcodes[0xD4] = [this]() { return Callcc(F_NC, ReadTwoBytes()); };

		opcodes[0xE0] = [this]() { return LdnA(ReadByte()); };
		opcodes[0xE2] = [this]() { return LdCA(); };

		opcodes[0xC5] = [this]() { return Push(BC); };
		opcodes[0xD5] = [this]() { return Push(DE); };
		opcodes[0xE5] = [this]() { return Push(HL); };
		opcodes[0xF5] = [this]() { return Push(AF); };

		opcodes[0xC6] = [this]() { return Add(ReadByte()); };
		opcodes[0xD6] = [this]() { return Sub(ReadByte()); };
		opcodes[0xE6] = [this]() { return And(ReadByte()); };
		opcodes[0xF6] = [this]() { return Or(ReadByte()); };

		opcodes[0xC7] = [this]() { return Rst(0x00); };
		opcodes[0xD7] = [this]() { return Rst(0x02); };
		opcodes[0xE7] = [this]() { return Rst(0x04); };
		opcodes[0xF7] = [this]() { return Rst(0x06); };

		opcodes[0xC8] = [this]() { return Retcc(F_Z); };
		opcodes[0xD8] = [this]() { return Retcc(F_C); };
		opcodes[0xE8] = [this]() { return AddSP(ReadByte()); };
		opcodes[0xF8] = [this]() { return LdHLSPe(ReadByte()); };

		opcodes[0xC9] = [this]() { return Ret(); };
		opcodes[0xD9] = [this]() { return RetI(); };
		opcodes[0xE9] = [this]() { return JpHL(); };
		opcodes[0xF9] = [this]() { return LdSPHL(); };

		opcodes[0xCA] = [this]() { return Jpcc(F_Z, ReadTwoBytes()); };
		opcodes[0xDA] = [this]() { return Jpcc(F_C, ReadTwoBytes()); };
		opcodes[0xEA] = [this]() { return LdnnA(ReadTwoBytes()); };
		opcodes[0xFA] = [this]() { return LdAnn(ReadTwoBytes()); };

		opcodes[0xFB] = [this]() { return Ei(); };

		opcodes[0xCC] = [this]() { return Callcc(F_Z, ReadTwoBytes()); };
		opcodes[0xDC] = [this]() { return Callcc(F_C, ReadTwoBytes()); };

		opcodes[0xCD] = [this]() { return Callnn(ReadTwoBytes()); };

		opcodes[0xCE] = [this]() { return Adc(ReadByte()); };
		opcodes[0xDE] = [this]() { return Sbc(ReadByte()); };
		opcodes[0xEE] = [this]() { return Xor(ReadByte()); };
		opcodes[0xFE] = [this]() { return Cmp(ReadByte()); };

		opcodes[0xCF] = [this]() { return Rst(0x01); };
		opcodes[0xDF] = [this]() { return Rst(0x03); };
		opcodes[0xEF] = [this]() { return Rst(0x05); };
		opcodes[0xFF] = [this]() { return Rst(0x07); };

		// 16-bit Opcodes
		opcodes_16[0x00] = [this]() { return Rlc(B); };
		opcodes_16[0x01] = [this]() { return Rlc(C); };
		opcodes_16[0x02] = [this]() { return Rlc(D); };
		opcodes_16[0x03] = [this]() { return Rlc(E); };
		opcodes_16[0x04] = [this]() { return Rlc(H); };
		opcodes_16[0x05] = [this]() { return Rlc(L); };
		opcodes_16[0x06] = [this]() { return RlcHL(); };
		opcodes_16[0x07] = [this]() { return Rlc(A); };
		opcodes_16[0x08] = [this]() { return Rrc(B); };
		opcodes_16[0x09] = [this]() { return Rrc(C); };
		opcodes_16[0x0A] = [this]() { return Rrc(D); };
		opcodes_16[0x0B] = [this]() { return Rrc(E); };
		opcodes_16[0x0C] = [this]() { return Rrc(H); };
		opcodes_16[0x0D] = [this]() { return Rrc(L); };
		opcodes_16[0x0E] = [this]() { return RrcHL(); };
		opcodes_16[0x0F] = [this]() { return Rrc(A); };

		opcodes_16[0x10] = [this]() { return Rl(B); };
		opcodes_16[0x11] = [this]() { return Rl(C); };
		opcodes_16[0x12] = [this]() { return Rl(D); };
		opcodes_16[0x13] = [this]() { return Rl(E); };
		opcodes_16[0x14] = [this]() { return Rl(H); };
		opcodes_16[0x15] = [this]() { return Rl(L); };
		opcodes_16[0x16] = [this]() { return RlHL(); };
		opcodes_16[0x17] = [this]() { return Rl(A); };
		opcodes_16[0x18] = [this]() { return Rr(B); };
		opcodes_16[0x19] = [this]() { return Rr(C); };
		opcodes_16[0x1A] = [this]() { return Rr(D); };
		opcodes_16[0x1B] = [this]() { return Rr(E); };
		opcodes_16[0x1C] = [this]() { return Rr(H); };
		opcodes_16[0x1D] = [this]() { return Rr(L); };
		opcodes_16[0x1E] = [this]() { return RrHL(); };
		opcodes_16[0x1F] = [this]() { return Rr(A); };

		opcodes_16[0x20] = [this]() { return Sla(B); };
		opcodes_16[0x21] = [this]() { return Sla(C); };
		opcodes_16[0x22] = [this]() { return Sla(D); };
		opcodes_16[0x23] = [this]() { return Sla(E); };
		opcodes_16[0x24] = [this]() { return Sla(H); };
		opcodes_16[0x25] = [this]() { return Sla(L); };
		opcodes_16[0x26] = [this]() { return SlaHL(); };
		opcodes_16[0x27] = [this]() { return Sla(A); };
		opcodes_16[0x28] = [this]() { return Sra(B); };
		opcodes_16[0x29] = [this]() { return Sra(C); };
		opcodes_16[0x2A] = [this]() { return Sra(D); };
		opcodes_16[0x2B] = [this]() { return Sra(E); };
		opcodes_16[0x2C] = [this]() { return Sra(H); };
		opcodes_16[0x2D] = [this]() { return Sra(L); };
		opcodes_16[0x2E] = [this]() { return SraHL(); };
		opcodes_16[0x2F] = [this]() { return Sra(A); };

		opcodes_16[0x30] = [this]() { return Swap(B); };
		opcodes_16[0x31] = [this]() { return Swap(C); };
		opcodes_16[0x32] = [this]() { return Swap(D); };
		opcodes_16[0x33] = [this]() { return Swap(E); };
		opcodes_16[0x34] = [this]() { return Swap(H); };
		opcodes_16[0x35] = [this]() { return Swap(L); };
		opcodes_16[0x36] = [this]() { return SwapHL(); };
		opcodes_16[0x37] = [this]() { return Swap(A); };
		opcodes_16[0x38] = [this]() { return Srl(B); };
		opcodes_16[0x39] = [this]() { return Srl(C); };
		opcodes_16[0x3A] = [this]() { return Srl(D); };
		opcodes_16[0x3B] = [this]() { return Srl(E); };
		opcodes_16[0x3C] = [this]() { return Srl(H); };
		opcodes_16[0x3D] = [this]() { return Srl(L); };
		opcodes_16[0x3E] = [this]() { return SrlHL(); };
		opcodes_16[0x3F] = [this]() { return Srl(A); };

		opcodes_16[0x40] = [this]() { return Bit(0, B); };
		opcodes_16[0x41] = [this]() { return Bit(0, C); };
		opcodes_16[0x42] = [this]() { return Bit(0, D); };
		opcodes_16[0x43] = [this]() { return Bit(0, E); };
		opcodes_16[0x44] = [this]() { return Bit(0, H); };
		opcodes_16[0x45] = [this]() { return Bit(0, L); };
		opcodes_16[0x46] = [this]() { return BitHL(0); };
		opcodes_16[0x47] = [this]() { return Bit(0, A); };
		opcodes_16[0x48] = [this]() { return Bit(1, B); };
		opcodes_16[0x49] = [this]() { return Bit(1, C); };
		opcodes_16[0x4A] = [this]() { return Bit(1, D); };
		opcodes_16[0x4B] = [this]() { return Bit(1, E); };
		opcodes_16[0x4C] = [this]() { return Bit(1, H); };
		opcodes_16[0x4D] = [this]() { return Bit(1, L); };
		opcodes_16[0x4E] = [this]() { return BitHL(1); };
		opcodes_16[0x4F] = [this]() { return Bit(1, A); };
		opcodes_16[0x50] = [this]() { return Bit(2, B); };
		opcodes_16[0x51] = [this]() { return Bit(2, C); };
		opcodes_16[0x52] = [this]() { return Bit(2, D); };
		opcodes_16[0x53] = [this]() { return Bit(2, E); };
		opcodes_16[0x54] = [this]() { return Bit(2, H); };
		opcodes_16[0x55] = [this]() { return Bit(2, L); };
		opcodes_16[0x56] = [this]() { return BitHL(2); };
		opcodes_16[0x57] = [this]() { return Bit(2, A); };
		opcodes_16[0x58] = [this]() { return Bit(3, B); };
		opcodes_16[0x59] = [this]() { return Bit(3, C); };
		opcodes_16[0x5A] = [this]() { return Bit(3, D); };
		opcodes_16[0x5B] = [this]() { return Bit(3, E); };
		opcodes_16[0x5C] = [this]() { return Bit(3, H); };
		opcodes_16[0x5D] = [this]() { return Bit(3, L); };
		opcodes_16[0x5E] = [this]() { return BitHL(3); };
		opcodes_16[0x5F] = [this]() { return Bit(3, A); };
		opcodes_16[0x60] = [this]() { return Bit(4, B); };
		opcodes_16[0x61] = [this]() { return Bit(4, C); };
		opcodes_16[0x62] = [this]() { return Bit(4, D); };
		opcodes_16[0x63] = [this]() { return Bit(4, E); };
		opcodes_16[0x64] = [this]() { return Bit(4, H); };
		opcodes_16[0x65] = [this]() { return Bit(4, L); };
		opcodes_16[0x66] = [this]() { return BitHL(4); };
		opcodes_16[0x67] = [this]() { return Bit(4, A); };
		opcodes_16[0x68] = [this]() { return Bit(5, B); };
		opcodes_16[0x69] = [this]() { return Bit(5, C); };
		opcodes_16[0x6A] = [this]() { return Bit(5, D); };
		opcodes_16[0x6B] = [this]() { return Bit(5, E); };
		opcodes_16[0x6C] = [this]() { return Bit(5, H); };
		opcodes_16[0x6D] = [this]() { return Bit(5, L); };
		opcodes_16[0x6E] = [this]() { return BitHL(5); };
		opcodes_16[0x6F] = [this]() { return Bit(5, A); };
		opcodes_16[0x70] = [this]() { return Bit(6, B); };
		opcodes_16[0x71] = [this]() { return Bit(6, C); };
		opcodes_16[0x72] = [this]() { return Bit(6, D); };
		opcodes_16[0x73] = [this]() { return Bit(6, E); };
		opcodes_16[0x74] = [this]() { return Bit(6, H); };
		opcodes_16[0x75] = [this]() { return Bit(6, L); };
		opcodes_16[0x76] = [this]() { return BitHL(6); };
		opcodes_16[0x77] = [this]() { return Bit(6, A); };
		opcodes_16[0x78] = [this]() { return Bit(7, B); };
		opcodes_16[0x79] = [this]() { return Bit(7, C); };
		opcodes_16[0x7A] = [this]() { return Bit(7, D); };
		opcodes_16[0x7B] = [this]() { return Bit(7, E); };
		opcodes_16[0x7C] = [this]() { return Bit(7, H); };
		opcodes_16[0x7D] = [this]() { return Bit(7, L); };
		opcodes_16[0x7E] = [this]() { return BitHL(7); };
		opcodes_16[0x7F] = [this]() { return Bit(7, A); };
		opcodes_16[0x80] = [this]() { return Res(0, B); };
		opcodes_16[0x81] = [this]() { return Res(0, C); };
		opcodes_16[0x82] = [this]() { return Res(0, D); };
		opcodes_16[0x83] = [this]() { return Res(0, E); };
		opcodes_16[0x84] = [this]() { return Res(0, H); };
		opcodes_16[0x85] = [this]() { return Res(0, L); };
		opcodes_16[0x86] = [this]() { return ResHL(0); };
		opcodes_16[0x87] = [this]() { return Res(0, A); };
		opcodes_16[0x88] = [this]() { return Res(1, B); };
		opcodes_16[0x89] = [this]() { return Res(1, C); };
		opcodes_16[0x8A] = [this]() { return Res(1, D); };
		opcodes_16[0x8B] = [this]() { return Res(1, E); };
		opcodes_16[0x8C] = [this]() { return Res(1, H); };
		opcodes_16[0x8D] = [this]() { return Res(1, L); };
		opcodes_16[0x8E] = [this]() { return ResHL(1); };
		opcodes_16[0x8F] = [this]() { return Res(1, A); };
		opcodes_16[0x90] = [this]() { return Res(2, B); };
		opcodes_16[0x91] = [this]() { return Res(2, C); };
		opcodes_16[0x92] = [this]() { return Res(2, D); };
		opcodes_16[0x93] = [this]() { return Res(2, E); };
		opcodes_16[0x94] = [this]() { return Res(2, H); };
		opcodes_16[0x95] = [this]() { return Res(2, L); };
		opcodes_16[0x96] = [this]() { return ResHL(2); };
		opcodes_16[0x97] = [this]() { return Res(2, A); };
		opcodes_16[0x98] = [this]() { return Res(3, B); };
		opcodes_16[0x99] = [this]() { return Res(3, C); };
		opcodes_16[0x9A] = [this]() { return Res(3, D); };
		opcodes_16[0x9B] = [this]() { return Res(3, E); };
		opcodes_16[0x9C] = [this]() { return Res(3, H); };
		opcodes_16[0x9D] = [this]() { return Res(3, L); };
		opcodes_16[0x9E] = [this]() { return ResHL(3); };
		opcodes_16[0x9F] = [this]() { return Res(3, A); };
		opcodes_16[0xA0] = [this]() { return Res(4, B); };
		opcodes_16[0xA1] = [this]() { return Res(4, C); };
		opcodes_16[0xA2] = [this]() { return Res(4, D); };
		opcodes_16[0xA3] = [this]() { return Res(4, E); };
		opcodes_16[0xA4] = [this]() { return Res(4, H); };
		opcodes_16[0xA5] = [this]() { return Res(4, L); };
		opcodes_16[0xA6] = [this]() { return ResHL(4); };
		opcodes_16[0xA7] = [this]() { return Res(4, A); };
		opcodes_16[0xA8] = [this]() { return Res(5, B); };
		opcodes_16[0xA9] = [this]() { return Res(5, C); };
		opcodes_16[0xAA] = [this]() { return Res(5, D); };
		opcodes_16[0xAB] = [this]() { return Res(5, E); };
		opcodes_16[0xAC] = [this]() { return Res(5, H); };
		opcodes_16[0xAD] = [this]() { return Res(5, L); };
		opcodes_16[0xAE] = [this]() { return ResHL(5); };
		opcodes_16[0xAF] = [this]() { return Res(5, A); };
		opcodes_16[0xB0] = [this]() { return Res(6, B); };
		opcodes_16[0xB1] = [this]() { return Res(6, C); };
		opcodes_16[0xB2] = [this]() { return Res(6, D); };
		opcodes_16[0xB3] = [this]() { return Res(6, E); };
		opcodes_16[0xB4] = [this]() { return Res(6, H); };
		opcodes_16[0xB5] = [this]() { return Res(6, L); };
		opcodes_16[0xB6] = [this]() { return ResHL(6); };
		opcodes_16[0xB7] = [this]() { return Res(6, A); };
		opcodes_16[0xB8] = [this]() { return Res(7, B); };
		opcodes_16[0xB9] = [this]() { return Res(7, C); };
		opcodes_16[0xBA] = [this]() { return Res(7, D); };
		opcodes_16[0xBB] = [this]() { return Res(7, E); };
		opcodes_16[0xBC] = [this]() { return Res(7, H); };
		opcodes_16[0xBD] = [this]() { return Res(7, L); };
		opcodes_16[0xBE] = [this]() { return ResHL(7); };
		opcodes_16[0xBF] = [this]() { return Res(7, A); };
		opcodes_16[0xC0] = [this]() { return Set(0, B); };
		opcodes_16[0xC1] = [this]() { return Set(0, C); };
		opcodes_16[0xC2] = [this]() { return Set(0, D); };
		opcodes_16[0xC3] = [this]() { return Set(0, E); };
		opcodes_16[0xC4] = [this]() { return Set(0, H); };
		opcodes_16[0xC5] = [this]() { return Set(0, L); };
		opcodes_16[0xC6] = [this]() { return SetHL(0); };
		opcodes_16[0xC7] = [this]() { return Set(0, A); };
		opcodes_16[0xC8] = [this]() { return Set(1, B); };
		opcodes_16[0xC9] = [this]() { return Set(1, C); };
		opcodes_16[0xCA] = [this]() { return Set(1, D); };
		opcodes_16[0xCB] = [this]() { return Set(1, E); };
		opcodes_16[0xCC] = [this]() { return Set(1, H); };
		opcodes_16[0xCD] = [this]() { return Set(1, L); };
		opcodes_16[0xCE] = [this]() { return SetHL(1); };
		opcodes_16[0xCF] = [this]() { return Set(1, A); };
		opcodes_16[0xD0] = [this]() { return Set(2, B); };
		opcodes_16[0xD1] = [this]() { return Set(2, C); };
		opcodes_16[0xD2] = [this]() { return Set(2, D); };
		opcodes_16[0xD3] = [this]() { return Set(2, E); };
		opcodes_16[0xD4] = [this]() { return Set(2, H); };
		opcodes_16[0xD5] = [this]() { return Set(2, L); };
		opcodes_16[0xD6] = [this]() { return SetHL(2); };
		opcodes_16[0xD7] = [this]() { return Set(2, A); };
		opcodes_16[0xD8] = [this]() { return Set(3, B); };
		opcodes_16[0xD9] = [this]() { return Set(3, C); };
		opcodes_16[0xDA] = [this]() { return Set(3, D); };
		opcodes_16[0xDB] = [this]() { return Set(3, E); };
		opcodes_16[0xDC] = [this]() { return Set(3, H); };
		opcodes_16[0xDD] = [this]() { return Set(3, L); };
		opcodes_16[0xDE] = [this]() { return SetHL(3); };
		opcodes_16[0xDF] = [this]() { return Set(3, A); };
		opcodes_16[0xE0] = [this]() { return Set(4, B); };
		opcodes_16[0xE1] = [this]() { return Set(4, C); };
		opcodes_16[0xE2] = [this]() { return Set(4, D); };
		opcodes_16[0xE3] = [this]() { return Set(4, E); };
		opcodes_16[0xE4] = [this]() { return Set(4, H); };
		opcodes_16[0xE5] = [this]() { return Set(4, L); };
		opcodes_16[0xE6] = [this]() { return SetHL(4); };
		opcodes_16[0xE7] = [this]() { return Set(4, A); };
		opcodes_16[0xE8] = [this]() { return Set(5, B); };
		opcodes_16[0xE9] = [this]() { return Set(5, C); };
		opcodes_16[0xEA] = [this]() { return Set(5, D); };
		opcodes_16[0xEB] = [this]() { return Set(5, E); };
		opcodes_16[0xEC] = [this]() { return Set(5, H); };
		opcodes_16[0xED] = [this]() { return Set(5, L); };
		opcodes_16[0xEE] = [this]() { return SetHL(5); };
		opcodes_16[0xEF] = [this]() { return Set(5, A); };
		opcodes_16[0xF0] = [this]() { return Set(6, B); };
		opcodes_16[0xF1] = [this]() { return Set(6, C); };
		opcodes_16[0xF2] = [this]() { return Set(6, D); };
		opcodes_16[0xF3] = [this]() { return Set(6, E); };
		opcodes_16[0xF4] = [this]() { return Set(6, H); };
		opcodes_16[0xF5] = [this]() { return Set(6, L); };
		opcodes_16[0xF6] = [this]() { return SetHL(6); };
		opcodes_16[0xF7] = [this]() { return Set(6, A); };
		opcodes_16[0xF8] = [this]() { return Set(7, B); };
		opcodes_16[0xF9] = [this]() { return Set(7, C); };
		opcodes_16[0xFA] = [this]() { return Set(7, D); };
		opcodes_16[0xFB] = [this]() { return Set(7, E); };
		opcodes_16[0xFC] = [this]() { return Set(7, H); };
		opcodes_16[0xFD] = [this]() { return Set(7, L); };
		opcodes_16[0xFE] = [this]() { return SetHL(7); };
		opcodes_16[0xFF] = [this]() { return Set(7, A); };

		// 8-Bit Opcode Strings
		opcode_strings[0x00] = "[this]() { return Nop(); };";
		opcode_strings[0x10] = "[this]() { return Stop(); };";
		opcode_strings[0x20] = "[this]() { return Jrecc(F_NZ, ReadByte()); };";
		opcode_strings[0x30] = "[this]() { return Jrecc(F_NC, ReadByte()); };";
		opcode_strings[0x01] = "[this]() { return Ldnn(BC, ReadTwoBytes()); };";
		opcode_strings[0x11] = "[this]() { return Ldnn(DE, ReadTwoBytes()); };";
		opcode_strings[0x21] = "[this]() { return Ldnn(HL, ReadTwoBytes()); };";
		opcode_strings[0x31] = "[this]() { return Ldnn(SP, ReadTwoBytes()); };";
		opcode_strings[0x02] = "[this]() { return LdBCA(); };";
		opcode_strings[0x12] = "[this]() { return LdDEA(); };";
		opcode_strings[0x22] = "[this]() { return LdHLIA(); };";
		opcode_strings[0x32] = "[this]() { return LdHLDA(); };";
		opcode_strings[0x03] = "[this]() { return Inc(BC); };";
		opcode_strings[0x13] = "[this]() { return Inc(DE); };";
		opcode_strings[0x23] = "[this]() { return Inc(HL); };";
		opcode_strings[0x33] = "[this]() { return Inc(SP); };";
		opcode_strings[0x04] = "[this]() { return Inc(B); };";
		opcode_strings[0x14] = "[this]() { return Inc(D); };";
		opcode_strings[0x24] = "[this]() { return Inc(H); };";
		opcode_strings[0x34] = "[this]() { return IncHL(); };";
		opcode_strings[0x05] = "[this]() { return Dec(B); };";
		opcode_strings[0x15] = "[this]() { return Dec(D); };";
		opcode_strings[0x25] = "[this]() { return Dec(H); };";
		opcode_strings[0x35] = "[this]() { return DecHL(); };";
		opcode_strings[0x06] = "[this]() { return Ld(B, ReadByte()); };";
		opcode_strings[0x16] = "[this]() { return Ld(D, ReadByte()); };";
		opcode_strings[0x26] = "[this]() { return Ld(H, ReadByte()); };";
		opcode_strings[0x36] = "[this]() { return LdHLn(ReadByte()); };";
		opcode_strings[0x07] = "[this]() { return RlcA(); };";
		opcode_strings[0x17] = "[this]() { return RlA(); };";
		opcode_strings[0x27] = "[this]() { return Daa(); };";
		opcode_strings[0x37] = "[this]() { return Scf(); };";
		opcode_strings[0x08] = "[this]() { return LdnnSP(ReadTwoBytes()); };";
		opcode_strings[0x18] = "[this]() { return Jre(ReadByte()); };";
		opcode_strings[0x28] = "[this]() { return Jrecc(F_Z, ReadByte()); };";
		opcode_strings[0x38] = "[this]() { return Jrecc(F_C, ReadByte()); };";
		opcode_strings[0x09] = "[this]() { return AddHL(BC); };";
		opcode_strings[0x19] = "[this]() { return AddHL(DE); };";
		opcode_strings[0x29] = "[this]() { return AddHL(HL); };";
		opcode_strings[0x39] = "[this]() { return AddHL(SP); };";
		opcode_strings[0x0A] = "[this]() { return LdABC(); };";
		opcode_strings[0x1A] = "[this]() { return LdADE(); };";
		opcode_strings[0x2A] = "[this]() { return LdAHLI(); };";
		opcode_strings[0x3A] = "[this]() { return LdAHLD(); };";
		opcode_strings[0x0B] = "[this]() { return Dec(BC); };";
		opcode_strings[0x1B] = "[this]() { return Dec(DE); };";
		opcode_strings[0x2B] = "[this]() { return Dec(HL); };";
		opcode_strings[0x3B] = "[this]() { return Dec(SP); };";
		opcode_strings[0x0C] = "[this]() { return Inc(C); };";
		opcode_strings[0x1C] = "[this]() { return Inc(E); };";
		opcode_strings[0x2C] = "[this]() { return Inc(L); };";
		opcode_strings[0x3C] = "[this]() { return Inc(A); };";
		opcode_strings[0x0D] = "[this]() { return Dec(C); };";
		opcode_strings[0x1D] = "[this]() { return Dec(E); };";
		opcode_strings[0x2D] = "[this]() { return Dec(L); };";
		opcode_strings[0x3D] = "[this]() { return Dec(A); };";
		opcode_strings[0x0E] = "[this]() { return Ld(C, ReadByte()); };";
		opcode_strings[0x1E] = "[this]() { return Ld(E, ReadByte()); };";
		opcode_strings[0x2E] = "[this]() { return Ld(L, ReadByte()); };";
		opcode_strings[0x3E] = "[this]() { return Ld(A, ReadByte()); };";
		opcode_strings[0x0F] = "[this]() { return RrcA(); };";
		opcode_strings[0x1F] = "[this]() { return RrA(); };";
		opcode_strings[0x2F] = "[this]() { return Cpl(); };";
		opcode_strings[0x3F] = "[this]() { return Ccf(); };";
		opcode_strings[0x40] = "[this]() { return Ld(B, B); };";
		opcode_strings[0x41] = "[this]() { return Ld(B, C); };";
		opcode_strings[0x42] = "[this]() { return Ld(B, D); };";
		opcode_strings[0x43] = "[this]() { return Ld(B, E); };";
		opcode_strings[0x44] = "[this]() { return Ld(B, H); };";
		opcode_strings[0x45] = "[this]() { return Ld(B, L); };";
		opcode_strings[0x46] = "[this]() { return LdrHL(B); };";
		opcode_strings[0x47] = "[this]() { return Ld(B, A); };";
		opcode_strings[0x48] = "[this]() { return Ld(C, B); };";
		opcode_strings[0x49] = "[this]() { return Ld(C, C); };";
		opcode_strings[0x4A] = "[this]() { return Ld(C, D); };";
		opcode_strings[0x4B] = "[this]() { return Ld(C, E); };";
		opcode_strings[0x4C] = "[this]() { return Ld(C, H); };";
		opcode_strings[0x4D] = "[this]() { return Ld(C, L); };";
		opcode_strings[0x4E] = "[this]() { return LdrHL(C); };";
		opcode_strings[0x4F] = "[this]() { return Ld(C, A); };";
		opcode_strings[0x50] = "[this]() { return Ld(D, B); };";
		opcode_strings[0x51] = "[this]() { return Ld(D, C); };";
		opcode_strings[0x52] = "[this]() { return Ld(D, D); };";
		opcode_strings[0x53] = "[this]() { return Ld(D, E); };";
		opcode_strings[0x54] = "[this]() { return Ld(D, H); };";
		opcode_strings[0x55] = "[this]() { return Ld(D, L); };";
		opcode_strings[0x56] = "[this]() { return LdrHL(D); };";
		opcode_strings[0x57] = "[this]() { return Ld(D, A); };";
		opcode_strings[0x58] = "[this]() { return Ld(E, B); };";
		opcode_strings[0x59] = "[this]() { return Ld(E, C); };";
		opcode_strings[0x5A] = "[this]() { return Ld(E, D); };";
		opcode_strings[0x5B] = "[this]() { return Ld(E, E); };";
		opcode_strings[0x5C] = "[this]() { return Ld(E, H); };";
		opcode_strings[0x5D] = "[this]() { return Ld(E, L); };";
		opcode_strings[0x5E] = "[this]() { return LdrHL(E); };";
		opcode_strings[0x5F] = "[this]() { return Ld(E, A); };";
		opcode_strings[0x60] = "[this]() { return Ld(H, B); };";
		opcode_strings[0x61] = "[this]() { return Ld(H, C); };";
		opcode_strings[0x62] = "[this]() { return Ld(H, D); };";
		opcode_strings[0x63] = "[this]() { return Ld(H, E); };";
		opcode_strings[0x64] = "[this]() { return Ld(H, H); };";
		opcode_strings[0x65] = "[this]() { return Ld(H, L); };";
		opcode_strings[0x66] = "[this]() { return LdrHL(H); };";
		opcode_strings[0x67] = "[this]() { return Ld(H, A); };";
		opcode_strings[0x68] = "[this]() { return Ld(L, B); };";
		opcode_strings[0x69] = "[this]() { return Ld(L, C); };";
		opcode_strings[0x6A] = "[this]() { return Ld(L, D); };";
		opcode_strings[0x6B] = "[this]() { return Ld(L, E); };";
		opcode_strings[0x6C] = "[this]() { return Ld(L, H); };";
		opcode_strings[0x6D] = "[this]() { return Ld(L, L); };";
		opcode_strings[0x6E] = "[this]() { return LdrHL(L); };";
		opcode_strings[0x6F] = "[this]() { return Ld(L, A); };";
		opcode_strings[0x70] = "[this]() { return LdHLr(B); };";
		opcode_strings[0x71] = "[this]() { return LdHLr(C); };";
		opcode_strings[0x72] = "[this]() { return LdHLr(D); };";
		opcode_strings[0x73] = "[this]() { return LdHLr(E); };";
		opcode_strings[0x74] = "[this]() { return LdHLr(H); };";
		opcode_strings[0x75] = "[this]() { return LdHLr(L); };";
		opcode_strings[0x76] = "[this]() { return Halt(); };";
		opcode_strings[0x77] = "[this]() { return LdHLr(A); };";
		opcode_strings[0x78] = "[this]() { return Ld(A, B); };";
		opcode_strings[0x79] = "[this]() { return Ld(A, C); };";
		opcode_strings[0x7A] = "[this]() { return Ld(A, D); };";
		opcode_strings[0x7B] = "[this]() { return Ld(A, E); };";
		opcode_strings[0x7C] = "[this]() { return Ld(A, H); };";
		opcode_strings[0x7D] = "[this]() { return Ld(A, L); };";
		opcode_strings[0x7E] = "[this]() { return LdrHL(A); };";
		opcode_strings[0x7F] = "[this]() { return Ld(A, A); };";
		opcode_strings[0x80] = "[this]() { return Add(B); };";
		opcode_strings[0x81] = "[this]() { return Add(C); };";
		opcode_strings[0x82] = "[this]() { return Add(D); };";
		opcode_strings[0x83] = "[this]() { return Add(E); };";
		opcode_strings[0x84] = "[this]() { return Add(H); };";
		opcode_strings[0x85] = "[this]() { return Add(L); };";
		opcode_strings[0x86] = "[this]() { return AddHL(); };";
		opcode_strings[0x87] = "[this]() { return Add(A); };";
		opcode_strings[0x88] = "[this]() { return Adc(B); };";
		opcode_strings[0x89] = "[this]() { return Adc(C); };";
		opcode_strings[0x8A] = "[this]() { return Adc(D); };";
		opcode_strings[0x8B] = "[this]() { return Adc(E); };";
		opcode_strings[0x8C] = "[this]() { return Adc(H); };";
		opcode_strings[0x8D] = "[this]() { return Adc(L); };";
		opcode_strings[0x8E] = "[this]() { return AdcHL(); };";
		opcode_strings[0x8F] = "[this]() { return Adc(A); };";
		opcode_strings[0x90] = "[this]() { return Sub(B); };";
		opcode_strings[0x91] = "[this]() { return Sub(C); };";
		opcode_strings[0x92] = "[this]() { return Sub(D); };";
		opcode_strings[0x93] = "[this]() { return Sub(E); };";
		opcode_strings[0x94] = "[this]() { return Sub(H); };";
		opcode_strings[0x95] = "[this]() { return Sub(L); };";
		opcode_strings[0x96] = "[this]() { return SubHL(); };";
		opcode_strings[0x97] = "[this]() { return Sub(A); };";
		opcode_strings[0x98] = "[this]() { return Sbc(B); };";
		opcode_strings[0x99] = "[this]() { return Sbc(C); };";
		opcode_strings[0x9A] = "[this]() { return Sbc(D); };";
		opcode_strings[0x9B] = "[this]() { return Sbc(E); };";
		opcode_strings[0x9C] = "[this]() { return Sbc(H); };";
		opcode_strings[0x9D] = "[this]() { return Sbc(L); };";
		opcode_strings[0x9E] = "[this]() { return SbcHL(); };";
		opcode_strings[0x9F] = "[this]() { return Sbc(A); };";
		opcode_strings[0xA0] = "[this]() { return And(B); };";
		opcode_strings[0xA1] = "[this]() { return And(C); };";
		opcode_strings[0xA2] = "[this]() { return And(D); };";
		opcode_strings[0xA3] = "[this]() { return And(E); };";
		opcode_strings[0xA4] = "[this]() { return And(H); };";
		opcode_strings[0xA5] = "[this]() { return And(L); };";
		opcode_strings[0xA6] = "[this]() { return AndHL(); };";
		opcode_strings[0xA7] = "[this]() { return And(A); };";
		opcode_strings[0xA8] = "[this]() { return Xor(B); };";
		opcode_strings[0xA9] = "[this]() { return Xor(C); };";
		opcode_strings[0xAA] = "[this]() { return Xor(D); };";
		opcode_strings[0xAB] = "[this]() { return Xor(E); };";
		opcode_strings[0xAC] = "[this]() { return Xor(H); };";
		opcode_strings[0xAD] = "[this]() { return Xor(L); };";
		opcode_strings[0xAE] = "[this]() { return XorHL(); };";
		opcode_strings[0xAF] = "[this]() { return Xor(A); };";
		opcode_strings[0xB0] = "[this]() { return Or(B); };";
		opcode_strings[0xB1] = "[this]() { return Or(C); };";
		opcode_strings[0xB2] = "[this]() { return Or(D); };";
		opcode_strings[0xB3] = "[this]() { return Or(E); };";
		opcode_strings[0xB4] = "[this]() { return Or(H); };";
		opcode_strings[0xB5] = "[this]() { return Or(L); };";
		opcode_strings[0xB6] = "[this]() { return OrHL(); };";
		opcode_strings[0xB7] = "[this]() { return Or(A); };";
		opcode_strings[0xB8] = "[this]() { return Cmp(B); };";
		opcode_strings[0xB9] = "[this]() { return Cmp(C); };";
		opcode_strings[0xBA] = "[this]() { return Cmp(D); };";
		opcode_strings[0xBB] = "[this]() { return Cmp(E); };";
		opcode_strings[0xBC] = "[this]() { return Cmp(H); };";
		opcode_strings[0xBD] = "[this]() { return Cmp(L); };";
		opcode_strings[0xBE] = "[this]() { return CmpHL(); };";
		opcode_strings[0xBF] = "[this]() { return Cmp(A); };";
		opcode_strings[0xC0] = "[this]() { return Retcc(F_NZ); };";
		opcode_strings[0xD0] = "[this]() { return Retcc(F_NC); };";
		opcode_strings[0xE0] = "[this]() { return LdnA(ReadByte()); };";
		opcode_strings[0xF0] = "[this]() { return LdAn(ReadByte()); };";
		opcode_strings[0xC1] = "[this]() { return Pop(BC); };";
		opcode_strings[0xD1] = "[this]() { return Pop(DE); };";
		opcode_strings[0xE1] = "[this]() { return Pop(HL); };";
		opcode_strings[0xF1] = "[this]() { return Pop(SP); };";
		opcode_strings[0xC2] = "[this]() { return Jpcc(F_NZ, ReadTwoBytes()); };";
		opcode_strings[0xD2] = "[this]() { return Jpcc(F_NC, ReadTwoBytes()); };";
		opcode_strings[0xE2] = "[this]() { return LdCA(); };";
		opcode_strings[0xF2] = "[this]() { return LdAC(); };";
		opcode_strings[0xC3] = "[this]() { return Jpnn(ReadTwoBytes()); };";
		opcode_strings[0xF3] = "[this]() { return Di(); };";
		opcode_strings[0xC4] = "[this]() { return Callcc(F_NZ, ReadTwoBytes()); };";
		opcode_strings[0xD4] = "[this]() { return Callcc(F_NC, ReadTwoBytes()); };";
		opcode_strings[0xE0] = "[this]() { return LdnA(ReadByte()); };";
		opcode_strings[0xE2] = "[this]() { return LdCA(); };";
		opcode_strings[0xC5] = "[this]() { return Push(BC); };";
		opcode_strings[0xD5] = "[this]() { return Push(DE); };";
		opcode_strings[0xE5] = "[this]() { return Push(HL); };";
		opcode_strings[0xF5] = "[this]() { return Push(AF); };";
		opcode_strings[0xC6] = "[this]() { return Add(ReadByte()); };";
		opcode_strings[0xD6] = "[this]() { return Sub(ReadByte()); };";
		opcode_strings[0xE6] = "[this]() { return And(ReadByte()); };";
		opcode_strings[0xF6] = "[this]() { return Or(ReadByte()); };";
		opcode_strings[0xC7] = "[this]() { return Rst(0x00); };";
		opcode_strings[0xD7] = "[this]() { return Rst(0x02); };";
		opcode_strings[0xE7] = "[this]() { return Rst(0x04); };";
		opcode_strings[0xF7] = "[this]() { return Rst(0x06); };";
		opcode_strings[0xC8] = "[this]() { return Retcc(F_Z); };";
		opcode_strings[0xD8] = "[this]() { return Retcc(F_C); };";
		opcode_strings[0xE8] = "[this]() { return AddSP(ReadByte()); };";
		opcode_strings[0xF8] = "[this]() { return LdHLSPe(ReadByte()); };";
		opcode_strings[0xC9] = "[this]() { return Ret(); };";
		opcode_strings[0xD9] = "[this]() { return RetI(); };";
		opcode_strings[0xE9] = "[this]() { return JpHL(); };";
		opcode_strings[0xF9] = "[this]() { return LdSPHL(); };";
		opcode_strings[0xCA] = "[this]() { return Jpcc(F_Z, ReadTwoBytes()); };";
		opcode_strings[0xDA] = "[this]() { return Jpcc(F_C, ReadTwoBytes()); };";
		opcode_strings[0xEA] = "[this]() { return LdnnA(ReadTwoBytes()); };";
		opcode_strings[0xFA] = "[this]() { return LdAnn(ReadTwoBytes()); };";
		opcode_strings[0xFB] = "[this]() { return Ei(); };";
		opcode_strings[0xCC] = "[this]() { return Callcc(F_Z, ReadTwoBytes()); };";
		opcode_strings[0xDC] = "[this]() { return Callcc(F_C, ReadTwoBytes()); };";
		opcode_strings[0xCD] = "[this]() { return Callnn(ReadTwoBytes()); };";
		opcode_strings[0xCE] = "[this]() { return Adc(ReadByte()); };";
		opcode_strings[0xDE] = "[this]() { return Sbc(ReadByte()); };";
		opcode_strings[0xEE] = "[this]() { return Xor(ReadByte()); };";
		opcode_strings[0xFE] = "[this]() { return Cmp(ReadByte()); };";
		opcode_strings[0xCF] = "[this]() { return Rst(0x01); };";
		opcode_strings[0xDF] = "[this]() { return Rst(0x03); };";
		opcode_strings[0xEF] = "[this]() { return Rst(0x05); };";
		opcode_strings[0xFF] = "[this]() { return Rst(0x07); };";

		// 16-bit Opcode Strings
		opcode_strings_16[0x00] = "[this]() { return Rlc(B); };";
		opcode_strings_16[0x01] = "[this]() { return Rlc(C); };";
		opcode_strings_16[0x02] = "[this]() { return Rlc(D); };";
		opcode_strings_16[0x03] = "[this]() { return Rlc(E); };";
		opcode_strings_16[0x04] = "[this]() { return Rlc(H); };";
		opcode_strings_16[0x05] = "[this]() { return Rlc(L); };";
		opcode_strings_16[0x06] = "[this]() { return RlcHL(); };";
		opcode_strings_16[0x07] = "[this]() { return Rlc(A); };";
		opcode_strings_16[0x08] = "[this]() { return Rrc(B); };";
		opcode_strings_16[0x09] = "[this]() { return Rrc(C); };";
		opcode_strings_16[0x0A] = "[this]() { return Rrc(D); };";
		opcode_strings_16[0x0B] = "[this]() { return Rrc(E); };";
		opcode_strings_16[0x0C] = "[this]() { return Rrc(H); };";
		opcode_strings_16[0x0D] = "[this]() { return Rrc(L); };";
		opcode_strings_16[0x0E] = "[this]() { return RrcHL(); };";
		opcode_strings_16[0x0F] = "[this]() { return Rrc(A); };";
		opcode_strings_16[0x10] = "[this]() { return Rl(B); };";
		opcode_strings_16[0x11] = "[this]() { return Rl(C); };";
		opcode_strings_16[0x12] = "[this]() { return Rl(D); };";
		opcode_strings_16[0x13] = "[this]() { return Rl(E); };";
		opcode_strings_16[0x14] = "[this]() { return Rl(H); };";
		opcode_strings_16[0x15] = "[this]() { return Rl(L); };";
		opcode_strings_16[0x16] = "[this]() { return RlHL(); };";
		opcode_strings_16[0x17] = "[this]() { return Rl(A); };";
		opcode_strings_16[0x18] = "[this]() { return Rr(B); };";
		opcode_strings_16[0x19] = "[this]() { return Rr(C); };";
		opcode_strings_16[0x1A] = "[this]() { return Rr(D); };";
		opcode_strings_16[0x1B] = "[this]() { return Rr(E); };";
		opcode_strings_16[0x1C] = "[this]() { return Rr(H); };";
		opcode_strings_16[0x1D] = "[this]() { return Rr(L); };";
		opcode_strings_16[0x1E] = "[this]() { return RrHL(); };";
		opcode_strings_16[0x1F] = "[this]() { return Rr(A); };";
		opcode_strings_16[0x20] = "[this]() { return Sla(B); };";
		opcode_strings_16[0x21] = "[this]() { return Sla(C); };";
		opcode_strings_16[0x22] = "[this]() { return Sla(D); };";
		opcode_strings_16[0x23] = "[this]() { return Sla(E); };";
		opcode_strings_16[0x24] = "[this]() { return Sla(H); };";
		opcode_strings_16[0x25] = "[this]() { return Sla(L); };";
		opcode_strings_16[0x26] = "[this]() { return SlaHL(); };";
		opcode_strings_16[0x27] = "[this]() { return Sla(A); };";
		opcode_strings_16[0x28] = "[this]() { return Sra(B); };";
		opcode_strings_16[0x29] = "[this]() { return Sra(C); };";
		opcode_strings_16[0x2A] = "[this]() { return Sra(D); };";
		opcode_strings_16[0x2B] = "[this]() { return Sra(E); };";
		opcode_strings_16[0x2C] = "[this]() { return Sra(H); };";
		opcode_strings_16[0x2D] = "[this]() { return Sra(L); };";
		opcode_strings_16[0x2E] = "[this]() { return SraHL(); };";
		opcode_strings_16[0x2F] = "[this]() { return Sra(A); };";
		opcode_strings_16[0x30] = "[this]() { return Swap(B); };";
		opcode_strings_16[0x31] = "[this]() { return Swap(C); };";
		opcode_strings_16[0x32] = "[this]() { return Swap(D); };";
		opcode_strings_16[0x33] = "[this]() { return Swap(E); };";
		opcode_strings_16[0x34] = "[this]() { return Swap(H); };";
		opcode_strings_16[0x35] = "[this]() { return Swap(L); };";
		opcode_strings_16[0x36] = "[this]() { return SwapHL(); };";
		opcode_strings_16[0x37] = "[this]() { return Swap(A); };";
		opcode_strings_16[0x38] = "[this]() { return Srl(B); };";
		opcode_strings_16[0x39] = "[this]() { return Srl(C); };";
		opcode_strings_16[0x3A] = "[this]() { return Srl(D); };";
		opcode_strings_16[0x3B] = "[this]() { return Srl(E); };";
		opcode_strings_16[0x3C] = "[this]() { return Srl(H); };";
		opcode_strings_16[0x3D] = "[this]() { return Srl(L); };";
		opcode_strings_16[0x3E] = "[this]() { return SrlHL(); };";
		opcode_strings_16[0x3F] = "[this]() { return Srl(A); };";
		opcode_strings_16[0x40] = "[this]() { return Bit(0, B); };";
		opcode_strings_16[0x41] = "[this]() { return Bit(0, C); };";
		opcode_strings_16[0x42] = "[this]() { return Bit(0, D); };";
		opcode_strings_16[0x43] = "[this]() { return Bit(0, E); };";
		opcode_strings_16[0x44] = "[this]() { return Bit(0, H); };";
		opcode_strings_16[0x45] = "[this]() { return Bit(0, L); };";
		opcode_strings_16[0x46] = "[this]() { return BitHL(0); };";
		opcode_strings_16[0x47] = "[this]() { return Bit(0, A); };";
		opcode_strings_16[0x48] = "[this]() { return Bit(1, B); };";
		opcode_strings_16[0x49] = "[this]() { return Bit(1, C); };";
		opcode_strings_16[0x4A] = "[this]() { return Bit(1, D); };";
		opcode_strings_16[0x4B] = "[this]() { return Bit(1, E); };";
		opcode_strings_16[0x4C] = "[this]() { return Bit(1, H); };";
		opcode_strings_16[0x4D] = "[this]() { return Bit(1, L); };";
		opcode_strings_16[0x4E] = "[this]() { return BitHL(1); };";
		opcode_strings_16[0x4F] = "[this]() { return Bit(1, A); };";
		opcode_strings_16[0x50] = "[this]() { return Bit(2, B); };";
		opcode_strings_16[0x51] = "[this]() { return Bit(2, C); };";
		opcode_strings_16[0x52] = "[this]() { return Bit(2, D); };";
		opcode_strings_16[0x53] = "[this]() { return Bit(2, E); };";
		opcode_strings_16[0x54] = "[this]() { return Bit(2, H); };";
		opcode_strings_16[0x55] = "[this]() { return Bit(2, L); };";
		opcode_strings_16[0x56] = "[this]() { return BitHL(2); };";
		opcode_strings_16[0x57] = "[this]() { return Bit(2, A); };";
		opcode_strings_16[0x58] = "[this]() { return Bit(3, B); };";
		opcode_strings_16[0x59] = "[this]() { return Bit(3, C); };";
		opcode_strings_16[0x5A] = "[this]() { return Bit(3, D); };";
		opcode_strings_16[0x5B] = "[this]() { return Bit(3, E); };";
		opcode_strings_16[0x5C] = "[this]() { return Bit(3, H); };";
		opcode_strings_16[0x5D] = "[this]() { return Bit(3, L); };";
		opcode_strings_16[0x5E] = "[this]() { return BitHL(3); };";
		opcode_strings_16[0x5F] = "[this]() { return Bit(3, A); };";
		opcode_strings_16[0x60] = "[this]() { return Bit(4, B); };";
		opcode_strings_16[0x61] = "[this]() { return Bit(4, C); };";
		opcode_strings_16[0x62] = "[this]() { return Bit(4, D); };";
		opcode_strings_16[0x63] = "[this]() { return Bit(4, E); };";
		opcode_strings_16[0x64] = "[this]() { return Bit(4, H); };";
		opcode_strings_16[0x65] = "[this]() { return Bit(4, L); };";
		opcode_strings_16[0x66] = "[this]() { return BitHL(4); };";
		opcode_strings_16[0x67] = "[this]() { return Bit(4, A); };";
		opcode_strings_16[0x68] = "[this]() { return Bit(5, B); };";
		opcode_strings_16[0x69] = "[this]() { return Bit(5, C); };";
		opcode_strings_16[0x6A] = "[this]() { return Bit(5, D); };";
		opcode_strings_16[0x6B] = "[this]() { return Bit(5, E); };";
		opcode_strings_16[0x6C] = "[this]() { return Bit(5, H); };";
		opcode_strings_16[0x6D] = "[this]() { return Bit(5, L); };";
		opcode_strings_16[0x6E] = "[this]() { return BitHL(5); };";
		opcode_strings_16[0x6F] = "[this]() { return Bit(5, A); };";
		opcode_strings_16[0x70] = "[this]() { return Bit(6, B); };";
		opcode_strings_16[0x71] = "[this]() { return Bit(6, C); };";
		opcode_strings_16[0x72] = "[this]() { return Bit(6, D); };";
		opcode_strings_16[0x73] = "[this]() { return Bit(6, E); };";
		opcode_strings_16[0x74] = "[this]() { return Bit(6, H); };";
		opcode_strings_16[0x75] = "[this]() { return Bit(6, L); };";
		opcode_strings_16[0x76] = "[this]() { return BitHL(6); };";
		opcode_strings_16[0x77] = "[this]() { return Bit(6, A); };";
		opcode_strings_16[0x78] = "[this]() { return Bit(7, B); };";
		opcode_strings_16[0x79] = "[this]() { return Bit(7, C); };";
		opcode_strings_16[0x7A] = "[this]() { return Bit(7, D); };";
		opcode_strings_16[0x7B] = "[this]() { return Bit(7, E); };";
		opcode_strings_16[0x7C] = "[this]() { return Bit(7, H); };";
		opcode_strings_16[0x7D] = "[this]() { return Bit(7, L); };";
		opcode_strings_16[0x7E] = "[this]() { return BitHL(7); };";
		opcode_strings_16[0x7F] = "[this]() { return Bit(7, A); };";
		opcode_strings_16[0x80] = "[this]() { return Res(0, B); };";
		opcode_strings_16[0x81] = "[this]() { return Res(0, C); };";
		opcode_strings_16[0x82] = "[this]() { return Res(0, D); };";
		opcode_strings_16[0x83] = "[this]() { return Res(0, E); };";
		opcode_strings_16[0x84] = "[this]() { return Res(0, H); };";
		opcode_strings_16[0x85] = "[this]() { return Res(0, L); };";
		opcode_strings_16[0x86] = "[this]() { return ResHL(0); };";
		opcode_strings_16[0x87] = "[this]() { return Res(0, A); };";
		opcode_strings_16[0x88] = "[this]() { return Res(1, B); };";
		opcode_strings_16[0x89] = "[this]() { return Res(1, C); };";
		opcode_strings_16[0x8A] = "[this]() { return Res(1, D); };";
		opcode_strings_16[0x8B] = "[this]() { return Res(1, E); };";
		opcode_strings_16[0x8C] = "[this]() { return Res(1, H); };";
		opcode_strings_16[0x8D] = "[this]() { return Res(1, L); };";
		opcode_strings_16[0x8E] = "[this]() { return ResHL(1); };";
		opcode_strings_16[0x8F] = "[this]() { return Res(1, A); };";
		opcode_strings_16[0x90] = "[this]() { return Res(2, B); };";
		opcode_strings_16[0x91] = "[this]() { return Res(2, C); };";
		opcode_strings_16[0x92] = "[this]() { return Res(2, D); };";
		opcode_strings_16[0x93] = "[this]() { return Res(2, E); };";
		opcode_strings_16[0x94] = "[this]() { return Res(2, H); };";
		opcode_strings_16[0x95] = "[this]() { return Res(2, L); };";
		opcode_strings_16[0x96] = "[this]() { return ResHL(2); };";
		opcode_strings_16[0x97] = "[this]() { return Res(2, A); };";
		opcode_strings_16[0x98] = "[this]() { return Res(3, B); };";
		opcode_strings_16[0x99] = "[this]() { return Res(3, C); };";
		opcode_strings_16[0x9A] = "[this]() { return Res(3, D); };";
		opcode_strings_16[0x9B] = "[this]() { return Res(3, E); };";
		opcode_strings_16[0x9C] = "[this]() { return Res(3, H); };";
		opcode_strings_16[0x9D] = "[this]() { return Res(3, L); };";
		opcode_strings_16[0x9E] = "[this]() { return ResHL(3); };";
		opcode_strings_16[0x9F] = "[this]() { return Res(3, A); };";
		opcode_strings_16[0xA0] = "[this]() { return Res(4, B); };";
		opcode_strings_16[0xA1] = "[this]() { return Res(4, C); };";
		opcode_strings_16[0xA2] = "[this]() { return Res(4, D); };";
		opcode_strings_16[0xA3] = "[this]() { return Res(4, E); };";
		opcode_strings_16[0xA4] = "[this]() { return Res(4, H); };";
		opcode_strings_16[0xA5] = "[this]() { return Res(4, L); };";
		opcode_strings_16[0xA6] = "[this]() { return ResHL(4); };";
		opcode_strings_16[0xA7] = "[this]() { return Res(4, A); };";
		opcode_strings_16[0xA8] = "[this]() { return Res(5, B); };";
		opcode_strings_16[0xA9] = "[this]() { return Res(5, C); };";
		opcode_strings_16[0xAA] = "[this]() { return Res(5, D); };";
		opcode_strings_16[0xAB] = "[this]() { return Res(5, E); };";
		opcode_strings_16[0xAC] = "[this]() { return Res(5, H); };";
		opcode_strings_16[0xAD] = "[this]() { return Res(5, L); };";
		opcode_strings_16[0xAE] = "[this]() { return ResHL(5); };";
		opcode_strings_16[0xAF] = "[this]() { return Res(5, A); };";
		opcode_strings_16[0xB0] = "[this]() { return Res(6, B); };";
		opcode_strings_16[0xB1] = "[this]() { return Res(6, C); };";
		opcode_strings_16[0xB2] = "[this]() { return Res(6, D); };";
		opcode_strings_16[0xB3] = "[this]() { return Res(6, E); };";
		opcode_strings_16[0xB4] = "[this]() { return Res(6, H); };";
		opcode_strings_16[0xB5] = "[this]() { return Res(6, L); };";
		opcode_strings_16[0xB6] = "[this]() { return ResHL(6); };";
		opcode_strings_16[0xB7] = "[this]() { return Res(6, A); };";
		opcode_strings_16[0xB8] = "[this]() { return Res(7, B); };";
		opcode_strings_16[0xB9] = "[this]() { return Res(7, C); };";
		opcode_strings_16[0xBA] = "[this]() { return Res(7, D); };";
		opcode_strings_16[0xBB] = "[this]() { return Res(7, E); };";
		opcode_strings_16[0xBC] = "[this]() { return Res(7, H); };";
		opcode_strings_16[0xBD] = "[this]() { return Res(7, L); };";
		opcode_strings_16[0xBE] = "[this]() { return ResHL(7); };";
		opcode_strings_16[0xBF] = "[this]() { return Res(7, A); };";
		opcode_strings_16[0xC0] = "[this]() { return Set(0, B); };";
		opcode_strings_16[0xC1] = "[this]() { return Set(0, C); };";
		opcode_strings_16[0xC2] = "[this]() { return Set(0, D); };";
		opcode_strings_16[0xC3] = "[this]() { return Set(0, E); };";
		opcode_strings_16[0xC4] = "[this]() { return Set(0, H); };";
		opcode_strings_16[0xC5] = "[this]() { return Set(0, L); };";
		opcode_strings_16[0xC6] = "[this]() { return SetHL(0); };";
		opcode_strings_16[0xC7] = "[this]() { return Set(0, A); };";
		opcode_strings_16[0xC8] = "[this]() { return Set(1, B); };";
		opcode_strings_16[0xC9] = "[this]() { return Set(1, C); };";
		opcode_strings_16[0xCA] = "[this]() { return Set(1, D); };";
		opcode_strings_16[0xCB] = "[this]() { return Set(1, E); };";
		opcode_strings_16[0xCC] = "[this]() { return Set(1, H); };";
		opcode_strings_16[0xCD] = "[this]() { return Set(1, L); };";
		opcode_strings_16[0xCE] = "[this]() { return SetHL(1); };";
		opcode_strings_16[0xCF] = "[this]() { return Set(1, A); };";
		opcode_strings_16[0xD0] = "[this]() { return Set(2, B); };";
		opcode_strings_16[0xD1] = "[this]() { return Set(2, C); };";
		opcode_strings_16[0xD2] = "[this]() { return Set(2, D); };";
		opcode_strings_16[0xD3] = "[this]() { return Set(2, E); };";
		opcode_strings_16[0xD4] = "[this]() { return Set(2, H); };";
		opcode_strings_16[0xD5] = "[this]() { return Set(2, L); };";
		opcode_strings_16[0xD6] = "[this]() { return SetHL(2); };";
		opcode_strings_16[0xD7] = "[this]() { return Set(2, A); };";
		opcode_strings_16[0xD8] = "[this]() { return Set(3, B); };";
		opcode_strings_16[0xD9] = "[this]() { return Set(3, C); };";
		opcode_strings_16[0xDA] = "[this]() { return Set(3, D); };";
		opcode_strings_16[0xDB] = "[this]() { return Set(3, E); };";
		opcode_strings_16[0xDC] = "[this]() { return Set(3, H); };";
		opcode_strings_16[0xDD] = "[this]() { return Set(3, L); };";
		opcode_strings_16[0xDE] = "[this]() { return SetHL(3); };";
		opcode_strings_16[0xDF] = "[this]() { return Set(3, A); };";
		opcode_strings_16[0xE0] = "[this]() { return Set(4, B); };";
		opcode_strings_16[0xE1] = "[this]() { return Set(4, C); };";
		opcode_strings_16[0xE2] = "[this]() { return Set(4, D); };";
		opcode_strings_16[0xE3] = "[this]() { return Set(4, E); };";
		opcode_strings_16[0xE4] = "[this]() { return Set(4, H); };";
		opcode_strings_16[0xE5] = "[this]() { return Set(4, L); };";
		opcode_strings_16[0xE6] = "[this]() { return SetHL(4); };";
		opcode_strings_16[0xE7] = "[this]() { return Set(4, A); };";
		opcode_strings_16[0xE8] = "[this]() { return Set(5, B); };";
		opcode_strings_16[0xE9] = "[this]() { return Set(5, C); };";
		opcode_strings_16[0xEA] = "[this]() { return Set(5, D); };";
		opcode_strings_16[0xEB] = "[this]() { return Set(5, E); };";
		opcode_strings_16[0xEC] = "[this]() { return Set(5, H); };";
		opcode_strings_16[0xED] = "[this]() { return Set(5, L); };";
		opcode_strings_16[0xEE] = "[this]() { return SetHL(5); };";
		opcode_strings_16[0xEF] = "[this]() { return Set(5, A); };";
		opcode_strings_16[0xF0] = "[this]() { return Set(6, B); };";
		opcode_strings_16[0xF1] = "[this]() { return Set(6, C); };";
		opcode_strings_16[0xF2] = "[this]() { return Set(6, D); };";
		opcode_strings_16[0xF3] = "[this]() { return Set(6, E); };";
		opcode_strings_16[0xF4] = "[this]() { return Set(6, H); };";
		opcode_strings_16[0xF5] = "[this]() { return Set(6, L); };";
		opcode_strings_16[0xF6] = "[this]() { return SetHL(6); };";
		opcode_strings_16[0xF7] = "[this]() { return Set(6, A); };";
		opcode_strings_16[0xF8] = "[this]() { return Set(7, B); };";
		opcode_strings_16[0xF9] = "[this]() { return Set(7, C); };";
		opcode_strings_16[0xFA] = "[this]() { return Set(7, D); };";
		opcode_strings_16[0xFB] = "[this]() { return Set(7, E); };";
		opcode_strings_16[0xFC] = "[this]() { return Set(7, H); };";
		opcode_strings_16[0xFD] = "[this]() { return Set(7, L); };";
		opcode_strings_16[0xFE] = "[this]() { return SetHL(7); };";
		opcode_strings_16[0xFF] = "[this]() { return Set(7, A); };";
	}

    ~Cpu();

    // Executes and returns the number of machine cycles to perform the next instruction
    int Tick();
};