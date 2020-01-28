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

int Cpu::AddCommon(u8 value, bool addCarry)
{
    F.ClearAllFlags();

    int flags = 0;
    u8 mask = 0b00001111;
    u8 valNibble = value & mask;
    u8 ANibble = A & mask;

    if (valNibble + ANibble > mask)
    {
        flags += RegisterU8::HCARRY_FLAG;
    }

    if (A > UINT8_MAX - value)
    {
        flags += RegisterU8::CARRY_FLAG;
    }

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

    reg++;

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

    reg--;

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