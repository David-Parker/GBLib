#pragma once
#include <stdexcept>
#include <fstream>
#include <iostream>
#include "Address.h"
#include "GlobalDefinitions.h"

class Memory
{
private:
    Byte RAM[Address::ADDRESSSPACE] = {};

public:
    Memory();
    ~Memory();
    void ClearMemory();
    void Write(Address address, Byte value);
    Byte Read(Address address);
    void Dump(Address start, Address end);
};