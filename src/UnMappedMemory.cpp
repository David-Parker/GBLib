#include "UnMappedMemory.h"

UnMappedMemory::UnMappedMemory()
{
}


UnMappedMemory::~UnMappedMemory()
{
}

void UnMappedMemory::Write(Address address, Byte value)
{
    throw std::exception("Cannot write, memory address is unmapped.");
}

Byte UnMappedMemory::Read(Address address)
{
    throw std::exception("Cannot read, memory address is unmapped.");
}