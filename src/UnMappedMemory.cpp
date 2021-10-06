#include "UnMappedMemory.h"

UnMappedMemory::UnMappedMemory()
{
}


UnMappedMemory::~UnMappedMemory()
{
}

void UnMappedMemory::Write(Address address, Byte value)
{
    // Cannot throw an exception because many games incorrectly try to access unmapped memory.
}

Byte UnMappedMemory::Read(Address address)
{
    // Cannot throw an exception because many games incorrectly try to access unmapped memory.
    return INVALID_READ;
}