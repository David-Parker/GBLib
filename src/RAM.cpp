#include "RAM.h"
#include "Address.h"

RAM::RAM(Address start, Address end)
{
    this->start = start;
    this->end = end;
    this->mem = (Byte*)calloc(((size_t)end - start) + 1, sizeof(Byte));
    this->canAccess = true;
}

RAM::~RAM()
{
    free(this->mem);
}

void RAM::Write(Address address, Byte value)
{
    if (!this->canAccess)
    {
        return;
    }

    this->mem[address - this->start] = value;
}

void RAM::EnableAccess()
{
    this->canAccess = true;
}

void RAM::DisableAccess()
{
    this->canAccess = false;
}