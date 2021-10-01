#include "GlobalDefinitions.h"
#include "ROM.h"
#include <cstdlib>

ROM::ROM(std::string name, int offset, int size)
{
    this->name = name;
    this->offset = offset;
    this->mem = (Byte*)malloc(size);
}

ROM::~ROM()
{
}

void ROM::DisableWrites()
{
    this->writeDisabled = true;
}

void ROM::Write(Address address, Byte value)
{
    if (this->writeDisabled)
    {
        throw std::exception("Cannot write to Read Only Memory.");
    }

    this->mem[address - offset] = value;
}

Byte ROM::Read(Address address)
{
    return this->mem[address - offset];
}