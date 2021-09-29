#pragma once
#include "GlobalDefinitions.h"

#define ADDR_IF 0xFF0F
#define ADDR_IE 0xFFFF
#define ADDR_NR10 0xFF10
#define ADDR_NR52 0xFF26
#define ADDR_LCDC 0xFF40
#define ADDR_WX 0xFF4b

class Address
{
private:
    u16 store;
public:
    static const int ADDRESSSPACE = 65536;
    Address(u16 address) : store(address) {}
    Address() : store(0x0) {}

    bool InRange(Address start, Address end);

    operator unsigned short() const
    {
        return store;
    }

    // postfix x++
    Address operator++(int)
    {
        Address copy(*this);
        operator++();
        return copy;
    }

    // prefix ++x
    Address& operator++()
    {
        store++;
        return *this;
    }

    ~Address() {}
};

inline bool Address::InRange(Address start, Address end)
{
    return store >= start.store && store <= end.store;
}