#include "Cpu.h"


Cpu::~Cpu()
{
}

int Cpu::Tick()
{
    // Fetch
    // Decode
    // Execute

    return 1;
}

u16 Cpu::Combine(u8 high, u8 low)
{
    return (high << 4) + low;
}

bool Cpu::CarryU8(u8 lhs, u8 rhs)
{
    if (lhs > UINT8_MAX - rhs)
    {
        return true;
    }
    else
    {
        return false;
    }
}

bool Cpu::HCarryU8(u8 lhs, u8 rhs)
{
    u8 mask = 0b00001111;
    u8 lhsNibble = lhs & mask;
    u8 rhsNibble = rhs & mask;

    if (lhsNibble + rhsNibble > mask)
    {
        return true;
    }
    else
    {
        return false;
    }
}

bool Cpu::CarryU16(u16 lhs, u16 rhs)
{
    if (lhs > UINT16_MAX - rhs)
    {
        return true;
    }
    else
    {
        return false;
    }
}

bool Cpu::HCarryU16(u16 lhs, u16 rhs)
{
    if (lhs + rhs > 0x00FF)
    {
        return true;
    }
    else
    {
        return false;
    }
}

bool Cpu::CarryS8(u16 lhs, s8 rhs)
{
    if (lhs > UINT16_MAX - rhs)
    {
        return true;
    }
    else
    {
        return false;
    }
}

bool Cpu::HCarryS8(u16 lhs, s8 rhs)
{
    if (lhs + rhs > 0x00FF)
    {
        return true;
    }
    else
    {
        return false;
    }
}

int Cpu::Nop()
{
    return 0;
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
    src = pMemory->ReadValue(HL);

    return 2;
}

int Cpu::LdHLr(RegisterU8& src)
{
    pMemory->StoreValue(HL, src);

    return 2;
}

int Cpu::LdHLr(u8 value)
{
    pMemory->StoreValue(HL, value);

    return 3;
}

int Cpu::LdABC()
{
    A = pMemory->ReadValue(BC);

    return 2;
}

int Cpu::LdADE()
{
    A = pMemory->ReadValue(DE);

    return 2;
}

int Cpu::LdAC()
{
    A = pMemory->ReadValue(0xFF00 + C);

    return 2;
}

int Cpu::LdCA()
{
    pMemory->StoreValue(0xFF00 + C, A);

    return 2;
}

int Cpu::LdAn(u8 value)
{
    A = pMemory->ReadValue(0xFF00 + value);

    return 3;
}

int Cpu::LdnA(u8 value)
{
    pMemory->StoreValue(0xFF00 + value, A);

    return 3;
}

int Cpu::LdAnn(u16 value)
{
    A = pMemory->ReadValue(value);

    return 4;
}

int Cpu::LdnnA(u16 value)
{
    pMemory->StoreValue(value, A);

    return 4;
}

int Cpu::LdAHLI()
{
    A = pMemory->ReadValue(HL);
    HL++;

    return 2;
}

int Cpu::LdAHLD()
{
    A = pMemory->ReadValue(HL);
    HL--;

    return 2;
}

int Cpu::LdBCA()
{
    pMemory->StoreValue(BC, A);

    return 2;
}

int Cpu::LdDEA()
{
    pMemory->StoreValue(DE, A);

    return 2;
}

int Cpu::LdHLIA()
{
    pMemory->StoreValue(HL, A);
    HL++;

    return 2;
}

int Cpu::LdHLDA()
{
    pMemory->StoreValue(HL, A);
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
    pMemory->StoreValue(--SP, reg.GetHighByte());
    pMemory->StoreValue(--SP, reg.GetLowByte());

    return 4;
}

int Cpu::Pop(RegisterU16& reg)
{
    reg.SetLowByte(pMemory->ReadValue(SP++));
    reg.SetHighByte(pMemory->ReadValue(SP++));

    return 3;
}

int Cpu::LdHLSPe(s8 value)
{
    F.ClearAllFlags();

    int flags = 0;
    u8 spLow = SP.GetLowByte();

    if (spLow + value > 0x00FF)
    {
        flags += RegisterU8::HCARRY_FLAG;
    }

    if (SP > UINT16_MAX - value)
    {
        flags += RegisterU8::CARRY_FLAG;
    }

    HL = SP + value;

    F.SetFlags(flags);

    return 3;
}

int Cpu::LdnnSP(u16 value)
{
    pMemory->StoreValue(value, SP.GetLowByte());
    pMemory->StoreValue(value + 1, SP.GetHighByte());

    return 5;
}

int Cpu::AddHL(RegisterU16& reg)
{
    F.ClearFlags(RegisterU8::CARRY_FLAG | RegisterU8::HCARRY_FLAG | RegisterU8::SUB_FLAG);

    int flags = 0;
    flags += HCarryU16(reg, HL) ? RegisterU8::HCARRY_FLAG : 0;
    flags += CarryU16(reg, HL) ? RegisterU8::CARRY_FLAG : 0;

    HL += reg;

    F.SetFlags(flags);

    return 2;
}

int Cpu::AddSP(s8 value)
{
    F.ClearAllFlags();

    int flags = 0;
    flags += HCarryS8(value, HL) ? RegisterU8::HCARRY_FLAG : 0;
    flags += CarryS8(value, HL) ? RegisterU8::CARRY_FLAG : 0;

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

    int bit7 = (A & 0b10000000) >> 7;

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

    int bit7 = (A & 0b10000000) >> 7;

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

    int bit0 = A & 0b00000001;

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

    int bit0 = A & 0b00000001;

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

    int bit7 = (reg & 0b10000000) >> 7;

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

    u8 value = pMemory->ReadValue(HL);

    int bit7 = (value & 0b10000000) >> 7;

    if (bit7 == 1)
    {
        F.SetFlags(RegisterU8::CARRY_FLAG);
    }

    value = (value << 1) | bit7;

    if (value == 0)
    {
        F.SetFlags(RegisterU8::ZERO_FLAG);
    }

    pMemory->StoreValue(HL, value);

    return 4;
}

int Cpu::Rl(RegisterU8& reg)
{
    int cyBit = F.FlagIsSet(RegisterU8::CARRY_FLAG) ? 1 : 0;

    F.ClearAllFlags();

    int bit7 = (reg & 0b10000000) >> 7;

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
    u8 value = pMemory->ReadValue(HL);

    F.ClearAllFlags();

    int bit7 = (value & 0b10000000) >> 7;

    if (bit7 == 1)
    {
        F.SetFlags(RegisterU8::CARRY_FLAG);
    }

    value = (value << 1) | cyBit;

    if (value == 0)
    {
        F.SetFlags(RegisterU8::ZERO_FLAG);
    }

    pMemory->StoreValue(HL, value);

    return 4;
}

int Cpu::Rrc(RegisterU8& reg)
{
    F.ClearAllFlags();

    int bit0 = reg & 0b00000001;

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
    u8 value = pMemory->ReadValue(HL);

    int bit0 = value & 0b00000001;

    if (bit0 == 1)
    {
        F.SetFlags(RegisterU8::CARRY_FLAG);
    }

    value = (value >> 1) | (bit0 << 7);

    if (value == 0)
    {
        F.SetFlags(RegisterU8::ZERO_FLAG);
    }

    pMemory->StoreValue(HL, value);

    return 4;
}

int Cpu::Rr(RegisterU8& reg)
{
    int cyBit = F.FlagIsSet(RegisterU8::CARRY_FLAG) ? 1 : 0;
    F.ClearAllFlags();

    int bit0 = reg & 0b00000001;

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
    u8 value = pMemory->ReadValue(HL);

    F.ClearAllFlags();

    int bit0 = value & 0b00000001;

    if (bit0 == 1)
    {
        F.SetFlags(RegisterU8::CARRY_FLAG);
    }

    value = (value >> 1) | (cyBit << 7);

    if (value == 0)
    {
        F.SetFlags(RegisterU8::ZERO_FLAG);
    }

    pMemory->StoreValue(HL, value);

    return 4;
}

int Cpu::Sla(RegisterU8& reg)
{
    F.ClearAllFlags();

    int bit7 = (reg & 0b10000000) >> 7;

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
    u8 value = pMemory->ReadValue(HL);

    int bit7 = (value & 0b10000000) >> 7;

    if (bit7 == 1)
    {
        F.SetFlags(RegisterU8::CARRY_FLAG);
    }

    value = value << 1;

    if (value == 0)
    {
        F.SetFlags(RegisterU8::ZERO_FLAG);
    }

    pMemory->StoreValue(HL, value);

    return 4;
}

int Cpu::Sra(RegisterU8& reg)
{
    F.ClearAllFlags();

    int bit0 = reg & 0b00000001;
    int bit7 = (reg & 0b10000000) >> 7;

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
    u8 value = pMemory->ReadValue(HL);

    int bit0 = value & 0b00000001;
    int bit7 = (value & 0b10000000) >> 7;

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

    pMemory->StoreValue(HL, value);

    return 4;
}

int Cpu::Srl(RegisterU8& reg)
{
    F.ClearAllFlags();

    int bit0 = reg & 0b00000001;

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
    u8 value = pMemory->ReadValue(HL);

    int bit0 = value & 0b00000001;

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

    pMemory->StoreValue(HL, value);

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

    u8 value = pMemory->ReadValue(HL);
    RegisterU8 val(&value);

    u8 low = val.GetLowNibble();
    u8 high = val.GetHighNibble();

    val.SetLowNibble(high);
    val.SetHighNibble(low);

    if (val == 0)
    {
        F.SetFlags(RegisterU8::ZERO_FLAG);
    }

    pMemory->StoreValue(HL, val);

    return 4;
}

int Cpu::Bit(RegisterU8& reg, u8 bit)
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

    u8 value = pMemory->ReadValue(HL);
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
    F.ClearAllFlags();

    int flags = 0;
    flags += HCarryU8(A, value) ? RegisterU8::HCARRY_FLAG : 0;
    flags += CarryU8(A, value) ? RegisterU8::CARRY_FLAG : 0;

    A += value;

    if (addCarry)
    {
        A += (flags & RegisterU8::CARRY_FLAG) >> 4;
    }

    if (A == 0)
    {
        flags += RegisterU8::ZERO_FLAG;
    }

    F.SetFlags(flags);

    return 2;
}

int Cpu::Set(RegisterU8& reg, u8 bit)
{
    reg.SetBit(bit);

    return 2;
}

int Cpu::SetHL(u8 bit)
{
    u8 value = pMemory->ReadValue(HL);
    RegisterU8 val(&value);
    val.SetBit(bit);
    pMemory->StoreValue(HL, val);

    return 4;
}

int Cpu::Res(RegisterU8& reg, u8 bit)
{
    reg.ResetBit(bit);

    return 2;
}

int Cpu::ResHL(u8 bit)
{
    u8 value = pMemory->ReadValue(HL);
    RegisterU8 val(&value);
    val.ResetBit(bit);
    pMemory->StoreValue(HL, val);

    return 4;
}

int Cpu::Jpnn(u16 value)
{
    PC = value;

    return 4;
}

int Cpu::Jpcc(u8 cc, u16 value)
{
    bool shouldJump = false;

    switch (cc)
    {
        case 0:
            if (!F.FlagIsSet(RegisterU8::ZERO_FLAG))
            {
                shouldJump = true;
            }
            break;
        case 1:
            if (F.FlagIsSet(RegisterU8::ZERO_FLAG))
            {
                shouldJump = true;
            }
            break;
        case 2:
            if (!F.FlagIsSet(RegisterU8::CARRY_FLAG))
            {
                shouldJump = true;
            }
            break;
        case 3:
            if (F.FlagIsSet(RegisterU8::CARRY_FLAG))
            {
                shouldJump = true;
            }
            break;
    }

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
    bool shouldJump = false;

    switch (cc)
    {
    case 0:
        if (!F.FlagIsSet(RegisterU8::ZERO_FLAG))
        {
            shouldJump = true;
        }
        break;
    case 1:
        if (F.FlagIsSet(RegisterU8::ZERO_FLAG))
        {
            shouldJump = true;
        }
        break;
    case 2:
        if (!F.FlagIsSet(RegisterU8::CARRY_FLAG))
        {
            shouldJump = true;
        }
        break;
    case 3:
        if (F.FlagIsSet(RegisterU8::CARRY_FLAG))
        {
            shouldJump = true;
        }
        break;
    }

    if (shouldJump)
    {
        PC += value;
        return 3;
    }

    return 2;
}

int Cpu::JpHL()
{
    PC = pMemory->ReadValue(HL);

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
    return AddCommon(pMemory->ReadValue(HL), false);
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
    return AddCommon(pMemory->ReadValue(HL), true);
}

int Cpu::SubCommon(u8 value, bool subCarry)
{
    F.ClearAllFlags();

    int flags = RegisterU8::SUB_FLAG;
    u8 mask = 0b00001111;
    u8 valNibble = value & mask;
    u8 ANibble = A & mask;

    if (valNibble > ANibble)
    {
        flags += RegisterU8::HCARRY_FLAG;
    }

    if (value > A)
    {
        flags += RegisterU8::CARRY_FLAG;
    }

    A -= value;

    if (subCarry)
    {
        A -= (flags & RegisterU8::CARRY_FLAG) >> 4;
    }

    if (A == 0)
    {
        flags += RegisterU8::ZERO_FLAG;
    }

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
    return SubCommon(pMemory->ReadValue(HL), false);
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
    return SubCommon(pMemory->ReadValue(HL), true);
}

int Cpu::And(RegisterU8& reg)
{
    return And(reg) / 2;
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
    return And(pMemory->ReadValue(HL));
}

int Cpu::Or(RegisterU8& reg)
{
    return Or(reg) / 2;
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
    return Or(pMemory->ReadValue(HL));
}

int Cpu::Xor(RegisterU8& reg)
{
    return Xor(reg) / 2;
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
    return Xor(pMemory->ReadValue(HL));
}

int Cpu::Cmp(RegisterU8& reg)
{
    return Cmp(reg) / 2;
}

int Cpu::Cmp(u8 value)
{
    F.ClearAllFlags();

    int flags = RegisterU8::SUB_FLAG;
    u8 mask = 0b00001111;
    u8 valNibble = value & mask;
    u8 ANibble = A & mask;

    if (valNibble > ANibble)
    {
        flags += RegisterU8::HCARRY_FLAG;
    }

    if (value > A)
    {
        flags += RegisterU8::CARRY_FLAG;
    }

    u8 result = A - value;

    if (result == 0)
    {
        flags += RegisterU8::ZERO_FLAG;
    }

    F.SetFlags(flags);

    return 2;
}

int Cpu::CmpHL()
{
    return Cmp(pMemory->ReadValue(HL));
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
    u8 value = pMemory->ReadValue(HL);

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

    pMemory->StoreValue(HL, value);

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
    u8 value = pMemory->ReadValue(HL);

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

    pMemory->StoreValue(HL, value);

    return 3;
}