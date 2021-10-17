#include "Address.h"

bool InRange(Address addr, Address start, Address end)
{
    return addr >= start && addr <= end;
}