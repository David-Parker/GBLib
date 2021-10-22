#include "Address.h"

bool AddressInRange(Address addr, Address start, Address end)
{
    return addr >= start && addr <= end;
}