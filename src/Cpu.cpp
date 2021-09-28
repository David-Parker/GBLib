#include "Cpu.h"

Cpu::~Cpu()
{
}

u8 Cpu::GetIF()
{
    return this->pMemory->Read(ADDR_IF);
}
u8 Cpu::GetIE()
{
    return this->pMemory->Read(ADDR_IE);
}


int Cpu::Tick()
{
#ifdef _DEBUG
    static bool enterStep = false;

    char sZOpCode[256];
    char sZRegisters[1024];

    // enterStep = enterStep || (PC == 0x0055); // 0x00e6

    u8 opcode_debug = this->pMemory->Read(PC);

    if (opcode_debug == 0xCB)
    {
        opcode_debug = this->pMemory->Read(PC + 1);
        snprintf(sZOpCode, 256, " -> PC: 0x%04X, Opcode: 0xCB%02X %s\n\n", *PC, opcode_debug, opcode_strings_16[opcode_debug]);
    }
    else
    {
        snprintf(sZOpCode, 256, " -> PC: 0x%04X, Opcode: 0x%04X %s\n\n", *PC, opcode_debug, opcode_strings[opcode_debug]);
    }

    FormatRegisters(sZRegisters, 1024);

    trace << sZRegisters;
    trace << sZOpCode;

    // Debug "breakpoint"
    if (enterStep)
    {
        //pMemory->Dump(0x8000, 0x9FFF);
        trace.flush();
        std::cout << sZRegisters;
        std::cout << sZOpCode;
        int x = 2;
    }
#endif

    instruction_t instruction;

    // Fetch
    u8 opcode = this->pMemory->Read(PC++);

    // Decode
    if (opcode == 0xCB) // 16-bit OP code switch
    {
        opcode = this->pMemory->Read(PC++);
        instruction = this->opcodes_16[opcode];
    }
    else
    {
        instruction = this->opcodes[opcode];
    }

    // Execute
    int cycles = instruction();

    if (cycles == -1)
    {
        // ERROR, unknown opcode.
        char msg[256];
        snprintf(msg, 256, "Unknown opcode encountered. Opcode: 0x02%X, PC: 0x04%X", opcode, *PC);
        throw std::exception(msg);
    }

    // Extra cycle for the opcode fetch
    return cycles + 1;
}

void Cpu::StartCPU()
{
#ifdef _DEBUG
    trace.open("trace.txt");
#endif

    running = true;
}

void Cpu::StopCPU()
{
#ifdef _DEBUG
    trace.flush();
#endif

    running = false;
}

bool Cpu::IsRunning()
{
    return running;
}

bool Cpu::FlagMatchesCC(u8 cc)
{
    bool matches = false;

    switch (cc)
    {
    case 0:
        if (!F.FlagIsSet(RegisterU8::ZERO_FLAG))
        {
            matches = true;
        }
        break;
    case 1:
        if (F.FlagIsSet(RegisterU8::ZERO_FLAG))
        {
            matches = true;
        }
        break;
    case 2:
        if (!F.FlagIsSet(RegisterU8::CARRY_FLAG))
        {
            matches = true;
        }
        break;
    case 3:
        if (F.FlagIsSet(RegisterU8::CARRY_FLAG))
        {
            matches = true;
        }
        break;
    default:
        throw std::exception("Invalid conditional.");
    }

    return matches;
}

u8 Cpu::ReadByte()
{
    u8 byte = this->pMemory->Read(PC++);

    return byte;
}

u16 Cpu::ReadTwoBytes()
{
    u16 lsb = (u16)ReadByte();
    u16 msb = (u16)ReadByte();

    return (msb << 8) + lsb;
}

void Cpu::FormatFlagsString(char* buf, int size)
{
    char* c_Z = F.FlagIsSet(RegisterU8::ZERO_FLAG) ? "Z" : "-";
    char* c_N = F.FlagIsSet(RegisterU8::SUB_FLAG) ? "N" : "-";
    char* c_H = F.FlagIsSet(RegisterU8::HCARRY_FLAG) ? "H" : "-";
    char* c_CY = F.FlagIsSet(RegisterU8::CARRY_FLAG) ? "CY" : "-";

    snprintf(buf, size, "%s %s %s %s", c_Z, c_N, c_H, c_CY);
}

void Cpu::FormatRegisters(char* buf, int n)
{
    char sZFlags[32];

    FormatFlagsString(sZFlags, 32);

    snprintf(buf, n, "BC:	0x%04X\nDE:	0x%04X\nHL:	0x%04X\nSP:	0x%04X\nAF:	0x%04X\nPC:	0x%04X\nB:	0x%02X\nC:	0x%02X\nD:	0x%02X\nE:	0x%02X\nH:	0x%02X\nL:	0x%02X\nA:	0x%02X\nF:	%s\nME:	0x%02X\n", *BC, *DE, *HL, *SP, *AF, *PC, *B, *C, *D, *E, *H, *L, *A, sZFlags, IME);
}

int Cpu::Daa()
{
    return -1;
}

int Cpu::Cpl()
{
    F.SetFlags(RegisterU8::HCARRY_FLAG | RegisterU8::SUB_FLAG);

    A = ~A;

    return 1;
}

int Cpu::Nop()
{
    return 1;
}

int Cpu::Ccf()
{
    F.ClearFlags(RegisterU8::HCARRY_FLAG | RegisterU8::SUB_FLAG);

    if (F.FlagIsSet(RegisterU8::CARRY_FLAG))
    {
        F.ClearFlags(RegisterU8::CARRY_FLAG);
    }
    else
    {
        F.SetFlags(RegisterU8::CARRY_FLAG);
    }

    return 1;
}

int Cpu::Scf()
{
    F.ClearFlags(RegisterU8::HCARRY_FLAG | RegisterU8::SUB_FLAG);

    F.SetFlags(RegisterU8::CARRY_FLAG);

    return 1;
}

int Cpu::Di()
{
    IME = 0;

    return 1;
}

int Cpu::Ei()
{
    IME = 1;

    return 1;
}

int Cpu::Halt()
{
    return -1;
}

int Cpu::Stop()
{
    return -1;
}

int Cpu::Ld(RegisterU8& dest, RegisterU8& src)
{
    dest = src;

    return 1;
}

int Cpu::Ld(RegisterU8& dest, u8 value)
{
    dest = value;

    return 2;
}

int Cpu::LdrHL(RegisterU8& src)
{
    src = pMemory->Read(HL);

    return 2;
}

int Cpu::LdHLr(RegisterU8& src)
{
    pMemory->Write(HL, src);

    return 2;
}

int Cpu::LdHLn(u8 value)
{
    pMemory->Write(HL, value);

    return 3;
}

int Cpu::LdABC()
{
    A = pMemory->Read(BC);

    return 2;
}

int Cpu::LdADE()
{
    A = pMemory->Read(DE);

    return 2;
}

int Cpu::LdAC()
{
    A = pMemory->Read(0xFF00 + C);

    return 2;
}

int Cpu::LdCA()
{
    pMemory->Write(0xFF00 + C, A);

    return 2;
}

int Cpu::LdAn(u8 value)
{
    A = pMemory->Read(0xFF00 + value);

    return 3;
}

int Cpu::LdnA(u8 value)
{
    pMemory->Write(0xFF00 + value, A);

    return 3;
}

int Cpu::LdAnn(u16 value)
{
    A = pMemory->Read(value);

    return 4;
}

int Cpu::LdnnA(u16 value)
{
    pMemory->Write(value, A);

    return 4;
}

int Cpu::LdAHLI()
{
    A = pMemory->Read(HL);
    HL++;

    return 2;
}

int Cpu::LdAHLD()
{
    A = pMemory->Read(HL);
    HL--;

    return 2;
}

int Cpu::LdBCA()
{
    pMemory->Write(BC, A);

    return 2;
}

int Cpu::LdDEA()
{
    pMemory->Write(DE, A);

    return 2;
}

int Cpu::LdHLIA()
{
    pMemory->Write(HL, A);
    HL++;

    return 2;
}

int Cpu::LdHLDA()
{
    pMemory->Write(HL, A);
    HL--;

    return 2;
}

int Cpu::Ldnn(RegisterU16& dest, u16 value)
{
    dest = value;

    return 3;
}

int Cpu::LdSPHL()
{
    SP = HL;

    return 2;
}

int Cpu::Push(RegisterU16& reg)
{
    pMemory->Write(--SP, reg.GetHighByte());
    pMemory->Write(--SP, reg.GetLowByte());

    return 4;
}

int Cpu::Pop(RegisterU16& reg)
{
    reg.SetLowByte(pMemory->Read(SP++));
    reg.SetHighByte(pMemory->Read(SP++));

    return 3;
}

int Cpu::LdHLSPe(s8 value)
{
    F.ClearAllFlags();

    int flags = 0;
    u16 result = SP + value;

    flags += ALUHelpers::Carry3Signed(SP, value) ? RegisterU8::HCARRY_FLAG : 0;
    flags += ALUHelpers::Carry7Signed(SP, value) ? RegisterU8::CARRY_FLAG : 0;

    HL = result;

    F.SetFlags(flags);

    return 3;
}

int Cpu::LdnnSP(u16 value)
{
    pMemory->Write(value, SP.GetLowByte());
    pMemory->Write(value + 1, SP.GetHighByte());

    return 5;
}

int Cpu::AddHL(RegisterU16& reg)
{
    F.ClearFlags(RegisterU8::CARRY_FLAG | RegisterU8::HCARRY_FLAG | RegisterU8::SUB_FLAG);

    int flags = 0;
    flags += ALUHelpers::Carry11Add(HL, reg) ? RegisterU8::HCARRY_FLAG : 0;
    flags += ALUHelpers::Carry15Add(HL, reg) ? RegisterU8::CARRY_FLAG : 0;

    HL += reg;

    F.SetFlags(flags);

    return 2;
}

int Cpu::AddSP(s8 value)
{
    F.ClearAllFlags();

    int flags = 0;
    flags += ALUHelpers::Carry3Signed(SP, value) ? RegisterU8::HCARRY_FLAG : 0;
    flags += ALUHelpers::Carry7Signed(SP, value) ? RegisterU8::CARRY_FLAG : 0;

    SP += value;

    F.SetFlags(flags);

    return 4;
}

int Cpu::Inc(RegisterU16& reg)
{
    reg++;

    return 2;
}

int Cpu::Dec(RegisterU16& reg)
{
    reg--;

    return 2;
}

int Cpu::RlcA()
{
    F.ClearAllFlags();

    int bit7 = A.GetBit(7);

    if (bit7 == 1)
    {
        F.SetFlags(RegisterU8::CARRY_FLAG);
    }

    A = (A << 1) | bit7;

    return 1;
}

int Cpu::RlA()
{
    int cyBit = F.FlagIsSet(RegisterU8::CARRY_FLAG) ? 1 : 0;
    F.ClearAllFlags();

    int bit7 = A.GetBit(7);

    if (bit7 == 1)
    {
        F.SetFlags(RegisterU8::CARRY_FLAG);
    }

    A = (A << 1) | cyBit;

    return 1;
}

int Cpu::RrcA()
{
    F.ClearAllFlags();

    int bit0 = A.GetBit(0);

    if (bit0 == 1)
    {
        F.SetFlags(RegisterU8::CARRY_FLAG);
    }

    A = (A >> 1) | (bit0 << 7);

    return 1;
}

int Cpu::RrA()
{
    int cyBit = F.FlagIsSet(RegisterU8::CARRY_FLAG) ? 1 : 0;
    F.ClearAllFlags();

    int bit0 = A.GetBit(0);

    if (bit0 == 1)
    {
        F.SetFlags(RegisterU8::CARRY_FLAG);
    }

    A = (A >> 1) | (cyBit << 7);

    return 1;
}

int Cpu::Rlc(RegisterU8& reg)
{
    F.ClearAllFlags();

    int bit7 = reg.GetBit(7);

    if (bit7 == 1)
    {
        F.SetFlags(RegisterU8::CARRY_FLAG);
    }

    reg = (reg << 1) | bit7;

    if (reg == 0)
    {
        F.SetFlags(RegisterU8::ZERO_FLAG);
    }

    return 2;
}

int Cpu::RlcHL()
{
    F.ClearAllFlags();

    u8 value = pMemory->Read(HL);

    int bit7 = (value >> 7) & 0x1;

    if (bit7 == 1)
    {
        F.SetFlags(RegisterU8::CARRY_FLAG);
    }

    value = (value << 1) | bit7;

    if (value == 0)
    {
        F.SetFlags(RegisterU8::ZERO_FLAG);
    }

    pMemory->Write(HL, value);

    return 4;
}

int Cpu::Rl(RegisterU8& reg)
{
    int cyBit = F.FlagIsSet(RegisterU8::CARRY_FLAG) ? 1 : 0;

    F.ClearAllFlags();

    int bit7 = reg.GetBit(7);

    if (bit7 == 1)
    {
        F.SetFlags(RegisterU8::CARRY_FLAG);
    }

    reg = (reg << 1) | cyBit;

    if (reg == 0)
    {
        F.SetFlags(RegisterU8::ZERO_FLAG);
    }

    return 2;
}

int Cpu::RlHL()
{
    int cyBit = F.FlagIsSet(RegisterU8::CARRY_FLAG) ? 1 : 0;
    u8 value = pMemory->Read(HL);

    F.ClearAllFlags();

    int bit7 = (value >> 7) & 0x01;

    if (bit7 == 1)
    {
        F.SetFlags(RegisterU8::CARRY_FLAG);
    }

    value = (value << 1) | cyBit;

    if (value == 0)
    {
        F.SetFlags(RegisterU8::ZERO_FLAG);
    }

    pMemory->Write(HL, value);

    return 4;
}

int Cpu::Rrc(RegisterU8& reg)
{
    F.ClearAllFlags();

    int bit0 = reg.GetBit(0);

    if (bit0 == 1)
    {
        F.SetFlags(RegisterU8::CARRY_FLAG);
    }

    reg = (reg >> 1) | (bit0 << 7);

    if (reg == 0)
    {
        F.SetFlags(RegisterU8::ZERO_FLAG);
    }

    return 2;
}

int Cpu::RrcHL()
{
    F.ClearAllFlags();
    u8 value = pMemory->Read(HL);

    int bit0 = value & 0x01;

    if (bit0 == 1)
    {
        F.SetFlags(RegisterU8::CARRY_FLAG);
    }

    value = (value >> 1) | (bit0 << 7);

    if (value == 0)
    {
        F.SetFlags(RegisterU8::ZERO_FLAG);
    }

    pMemory->Write(HL, value);

    return 4;
}

int Cpu::Rr(RegisterU8& reg)
{
    int cyBit = F.FlagIsSet(RegisterU8::CARRY_FLAG) ? 1 : 0;
    F.ClearAllFlags();

    int bit0 = reg.GetBit(0);

    if (bit0 == 1)
    {
        F.SetFlags(RegisterU8::CARRY_FLAG);
    }

    reg = (reg >> 1) | (cyBit << 7);

    if (reg == 0)
    {
        F.SetFlags(RegisterU8::ZERO_FLAG);
    }

    return 2;
}

int Cpu::RrHL()
{
    int cyBit = F.FlagIsSet(RegisterU8::CARRY_FLAG) ? 1 : 0;
    u8 value = pMemory->Read(HL);

    F.ClearAllFlags();

    int bit0 = value & 0x01;

    if (bit0 == 1)
    {
        F.SetFlags(RegisterU8::CARRY_FLAG);
    }

    value = (value >> 1) | (cyBit << 7);

    if (value == 0)
    {
        F.SetFlags(RegisterU8::ZERO_FLAG);
    }

    pMemory->Write(HL, value);

    return 4;
}

int Cpu::Sla(RegisterU8& reg)
{
    F.ClearAllFlags();

    int bit7 = reg.GetBit(7);

    if (bit7 == 1)
    {
        F.SetFlags(RegisterU8::CARRY_FLAG);
    }

    reg = reg << 1;

    if (reg == 0)
    {
        F.SetFlags(RegisterU8::ZERO_FLAG);
    }

    return 2;
}

int Cpu::SlaHL()
{
    F.ClearAllFlags();
    u8 value = pMemory->Read(HL);

    int bit7 = (value >> 7) & 0x01;

    if (bit7 == 1)
    {
        F.SetFlags(RegisterU8::CARRY_FLAG);
    }

    value = value << 1;

    if (value == 0)
    {
        F.SetFlags(RegisterU8::ZERO_FLAG);
    }

    pMemory->Write(HL, value);

    return 4;
}

int Cpu::Sra(RegisterU8& reg)
{
    F.ClearAllFlags();

    int bit0 = reg.GetBit(0);
    int bit7 = reg.GetBit(7);

    if (bit0 == 1)
    {
        F.SetFlags(RegisterU8::CARRY_FLAG);
    }

    reg = reg >> 1;
    reg = reg | bit7;

    if (reg == 0)
    {
        F.SetFlags(RegisterU8::ZERO_FLAG);
    }

    return 2;
}

int Cpu::SraHL()
{
    F.ClearAllFlags();
    u8 value = pMemory->Read(HL);

    int bit0 = value & 0x01;
    int bit7 = (value >> 7) & 0x01;

    if (bit0 == 1)
    {
        F.SetFlags(RegisterU8::CARRY_FLAG);
    }

    value = value >> 1;
    value = value | bit7;

    if (value == 0)
    {
        F.SetFlags(RegisterU8::ZERO_FLAG);
    }

    pMemory->Write(HL, value);

    return 4;
}

int Cpu::Srl(RegisterU8& reg)
{
    F.ClearAllFlags();

    int bit0 = reg.GetBit(0);

    if (bit0 == 1)
    {
        F.SetFlags(RegisterU8::CARRY_FLAG);
    }

    reg = reg >> 1;

    reg = reg & 0b01111111;

    if (reg == 0)
    {
        F.SetFlags(RegisterU8::ZERO_FLAG);
    }

    return 2;
}

int Cpu::SrlHL()
{
    F.ClearAllFlags();
    u8 value = pMemory->Read(HL);

    int bit0 = value & 0x01;

    if (bit0 == 1)
    {
        F.SetFlags(RegisterU8::CARRY_FLAG);
    }

    value = value >> 1;

    value = value & 0b01111111;

    if (value == 0)
    {
        F.SetFlags(RegisterU8::ZERO_FLAG);
    }

    pMemory->Write(HL, value);

    return 4;
}

int Cpu::Swap(RegisterU8& reg)
{
    F.ClearAllFlags();

    u8 low = reg.GetLowNibble();
    u8 high = reg.GetHighNibble();

    reg.SetLowNibble(high);
    reg.SetHighNibble(low);

    if (reg == 0)
    {
        F.SetFlags(RegisterU8::ZERO_FLAG);
    }

    return 2;
}

int Cpu::SwapHL()
{
    F.ClearAllFlags();

    u8 value = pMemory->Read(HL);
    RegisterU8 val(&value);

    u8 low = val.GetLowNibble();
    u8 high = val.GetHighNibble();

    val.SetLowNibble(high);
    val.SetHighNibble(low);

    if (val == 0)
    {
        F.SetFlags(RegisterU8::ZERO_FLAG);
    }

    pMemory->Write(HL, val);

    return 4;
}

int Cpu::Bit(u8 bit, RegisterU8& reg)
{
    F.ClearFlags(RegisterU8::SUB_FLAG | RegisterU8::ZERO_FLAG);
    int flags = RegisterU8::HCARRY_FLAG;

    int bVal = reg.GetBit(bit);
    int bValCom = bVal == 0 ? 1 : 0;

    if (bValCom == 1)
    {
        flags += RegisterU8::ZERO_FLAG;
    }

    F.SetFlags(flags);

    return 2;
}

int Cpu::BitHL(u8 bit)
{
    F.ClearFlags(RegisterU8::SUB_FLAG | RegisterU8::ZERO_FLAG);
    int flags = RegisterU8::HCARRY_FLAG;

    u8 value = pMemory->Read(HL);
    RegisterU8 val(&value);

    int bVal = val.GetBit(bit);
    int bValCom = bVal == 0 ? 1 : 0;

    if (bValCom == 1)
    {
        flags += RegisterU8::ZERO_FLAG;
    }

    F.SetFlags(flags);

    return 3;
}

int Cpu::AddCommon(u8 value, bool addCarry)
{
    u8 carry = 0;
    u8 flags = 0;

    if (addCarry && F.FlagIsSet(RegisterU8::CARRY_FLAG))
    {
        carry = 1;
    }

    F.ClearAllFlags();

    flags += ALUHelpers::Carry3Add(A, value, carry) ? RegisterU8::HCARRY_FLAG : 0;
    flags += ALUHelpers::Carry7Add(A, value, carry) ? RegisterU8::CARRY_FLAG : 0;

    A = A + value + carry;

    flags += A == 0 ? RegisterU8::ZERO_FLAG : 0;

    F.SetFlags(flags);

    return 2;
}

int Cpu::Set(u8 bit, RegisterU8& reg)
{
    reg.SetBit(bit);

    return 2;
}

int Cpu::SetHL(u8 bit)
{
    u8 value = pMemory->Read(HL);
    RegisterU8 val(&value);
    val.SetBit(bit);
    pMemory->Write(HL, val);

    return 4;
}

int Cpu::Res(u8 bit, RegisterU8& reg)
{
    reg.ResetBit(bit);

    return 2;
}

int Cpu::ResHL(u8 bit)
{
    u8 value = pMemory->Read(HL);
    RegisterU8 val(&value);
    val.ResetBit(bit);
    pMemory->Write(HL, val);

    return 4;
}

int Cpu::Jpnn(u16 value)
{
    PC = value;

    return 4;
}

int Cpu::Jpcc(u8 cc, u16 value)
{
    bool shouldJump = FlagMatchesCC(cc);

    if (shouldJump)
    {
        PC = value;
        return 4;
    }

    return 3;
}

int Cpu::Jre(s8 value)
{
    PC += value;

    return 3;
}

int Cpu::Jrecc(u8 cc, s8 value)
{
    bool shouldJump = FlagMatchesCC(cc);

    if (shouldJump)
    {
        return Jre(value);
    }

    return 2;
}

int Cpu::JpHL()
{
    PC = pMemory->Read(HL);

    return 1;
}

int Cpu::Add(RegisterU8& reg)
{
    return AddCommon(reg, false) / 2;
}

int Cpu::Add(u8 value)
{
    return AddCommon(value, false);
}

int Cpu::AddHL()
{
    return AddCommon(pMemory->Read(HL), false);
}

int Cpu::Adc(RegisterU8& reg)
{
    return AddCommon(reg, true) / 2;
}

int Cpu::Adc(u8 value)
{
    return AddCommon(value, true);
}

int Cpu::AdcHL()
{
    return AddCommon(pMemory->Read(HL), true);
}

int Cpu::SubCommon(u8 value, bool subCarry)
{
    u8 carry = 0;
    u8 flags = 0;

    if (subCarry && F.FlagIsSet(RegisterU8::CARRY_FLAG))
    {
        carry = 1;
    }

    F.ClearAllFlags();

    flags += ALUHelpers::Carry3Sub(A, value, carry) ? RegisterU8::HCARRY_FLAG : 0;
    flags += ALUHelpers::Carry7Sub(A, value, carry) ? RegisterU8::CARRY_FLAG : 0;

    A = A - value - carry;

    flags += A == 0 ? RegisterU8::ZERO_FLAG : 0;

    F.SetFlags(flags);

    return 2;
}

int Cpu::Sub(RegisterU8& reg)
{
    return SubCommon(reg, false) / 2;
}

int Cpu::Sub(u8 value)
{
    return SubCommon(value, false);
}

int Cpu::SubHL()
{
    return SubCommon(pMemory->Read(HL), false);
}

int Cpu::Sbc(RegisterU8& reg)
{
    return SubCommon(reg, true) / 2;
}

int Cpu::Sbc(u8 value)
{
    return SubCommon(value, true);
}

int Cpu::SbcHL()
{
    return SubCommon(pMemory->Read(HL), true);
}

int Cpu::And(RegisterU8& reg)
{
    return And(*reg) / 2;
}

int Cpu::And(u8 value)
{
    F.ClearAllFlags();

    int flags = RegisterU8::HCARRY_FLAG;

    A = A & value;

    if (A == 0)
    {
        flags += RegisterU8::ZERO_FLAG;
    }

    F.SetFlags(flags);

    return 2;
}

int Cpu::AndHL()
{
    return And(pMemory->Read(HL));
}

int Cpu::Or(RegisterU8& reg)
{
    return Or(*reg) / 2;
}

int Cpu::Or(u8 value)
{
    F.ClearAllFlags();

    A = A | value;

    if (A == 0)
    {
        F.SetFlags(RegisterU8::ZERO_FLAG);
    }

    return 2;
}

int Cpu::OrHL()
{
    return Or(pMemory->Read(HL));
}

int Cpu::Xor(RegisterU8& reg)
{
    return Xor(*reg) / 2;
}

int Cpu::Xor(u8 value)
{
    F.ClearAllFlags();

    A = A ^ value;

    if (A == 0)
    {
        F.SetFlags(RegisterU8::ZERO_FLAG);
    }

    return 2;
}

int Cpu::XorHL()
{
    return Xor(pMemory->Read(HL));
}

int Cpu::Cmp(RegisterU8& reg)
{
    return Cmp(*reg) / 2;
}

int Cpu::Cmp(u8 value)
{
    F.ClearAllFlags();

    u8 flags = RegisterU8::SUB_FLAG;
    u8 result = A - value;

    flags += ALUHelpers::Carry3Sub(A, value, 0) ? RegisterU8::HCARRY_FLAG : 0;
    flags += A < result ? RegisterU8::CARRY_FLAG : 0;
    flags += result == 0 ? RegisterU8::ZERO_FLAG : 0;

    F.SetFlags(flags);

    return 2;
}

int Cpu::CmpHL()
{
    return Cmp(pMemory->Read(HL));
}

int Cpu::Inc(RegisterU8& reg)
{
    F.ClearFlags(RegisterU8::HCARRY_FLAG | RegisterU8::SUB_FLAG | RegisterU8::ZERO_FLAG);

    int flags = 0;

    if (reg == 0b00001111)
    {
        flags += RegisterU8::HCARRY_FLAG;
    }

    ++reg;

    if (reg == 0)
    {
        flags += RegisterU8::ZERO_FLAG;
    }

    F.SetFlags(flags);

    return 1;
}

int Cpu::IncHL()
{
    F.ClearFlags(RegisterU8::HCARRY_FLAG | RegisterU8::SUB_FLAG | RegisterU8::ZERO_FLAG);

    int flags = 0;
    u8 value = pMemory->Read(HL);

    if (value == 0b00001111)
    {
        flags += RegisterU8::HCARRY_FLAG;
    }

    value++;

    if (value == 0)
    {
        flags += RegisterU8::ZERO_FLAG;
    }

    F.SetFlags(flags);

    pMemory->Write(HL, value);

    return 3;
}

int Cpu::Dec(RegisterU8& reg)
{
    F.ClearFlags(RegisterU8::HCARRY_FLAG | RegisterU8::ZERO_FLAG);

    int flags = RegisterU8::SUB_FLAG;

    if (reg == 0)
    {
        flags += RegisterU8::HCARRY_FLAG;
    }

    --reg;

    if (reg == 0)
    {
        flags += RegisterU8::ZERO_FLAG;
    }

    F.SetFlags(flags);

    return 1;
}

int Cpu::DecHL()
{
    F.ClearFlags(RegisterU8::HCARRY_FLAG | RegisterU8::ZERO_FLAG);

    int flags = RegisterU8::SUB_FLAG;
    u8 value = pMemory->Read(HL);

    if (value == 0)
    {
        flags += RegisterU8::HCARRY_FLAG;
    }

    value--;

    if (value == 0)
    {
        flags += RegisterU8::ZERO_FLAG;
    }

    F.SetFlags(flags);

    pMemory->Write(HL, value);

    return 3;
}

int Cpu::Callnn(u16 value)
{
    pMemory->Write(--SP, PC.GetHighByte());
    pMemory->Write(--SP, PC.GetLowByte());
    PC = value;

    return 6;
}

int Cpu::Callcc(u8 cc, u16 value)
{
    bool shouldJump = FlagMatchesCC(cc);

    if (shouldJump)
    {
        return Callnn(value);
    }

    return 3;
}

int Cpu::Ret()
{
    PC.SetLowByte(pMemory->Read(SP++));
    PC.SetHighByte(pMemory->Read(SP++));

    return 4;
}

int Cpu::RetI()
{
    IME = 1;

    return Ret();
}

int Cpu::Retcc(u8 cc)
{
    bool matches = FlagMatchesCC(cc);

    if (matches)
    {
        return Ret() + 1;
    }

    return 2;
}

int Cpu::Rst(u8 t)
{
    pMemory->Write(--SP, PC.GetHighByte());
    pMemory->Write(--SP, PC.GetLowByte());

    PC.SetHighByte(0);

    u8 address = 0;

    switch (t)
    {
    case 0:
        address = 0x00;
        break;
    case 1:
        address = 0x08;
        break;
    case 2:
        address = 0x10;
        break;
    case 3:
        address = 0x18;
        break;
    case 4:
        address = 0x20;
        break;
    case 5:
        address = 0x28;
        break;
    case 6:
        address = 0x30;
        break;
    case 7:
        address = 0x38;
        break;
    default:
        throw std::exception("Invalid memory location for operand t in Rst().");
    }

    PC.SetLowByte(address);

    return 4;
}