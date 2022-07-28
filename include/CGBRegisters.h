#pragma once
#include "GlobalDefinitions.h"
#include "Memory.h"
#include "IMemoryMappable.h"
#include "RegisterU8.h"

class CGBRegisters : public IMemoryMappable
{
private:
    Memory* pMemory;

    RegisterU8 VBK;
    RegisterU8 KEY1;
    RegisterU16 HDMASource;
    RegisterU16 HDMADest;
    RegisterU8 HDMA1;
    RegisterU8 HDMA2;
    RegisterU8 HDMA3;
    RegisterU8 HDMA4;
    RegisterU8 HDMA5;
    RegisterU8 RP;
    RegisterU8 SVBK;

public:
    CGBRegisters(Memory* pMemory);
    ~CGBRegisters() {}

    void Write(Address address, Byte value);
    Byte Read(Address address);
};