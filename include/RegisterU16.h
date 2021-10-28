#pragma once
#include "Address.h"
#include "GlobalDefinitions.h"

class RegisterU16
{
private:
    u16 value;

public:
    RegisterU16() : value(0x0) {}
    RegisterU16(u16 val) : value(val) {}
    ~RegisterU16() {}

    operator u16() const
    {
        return value;
    }

    int operator=(int num)
    {
        value = num;
        return num;
    }

    u16 operator*()
    {
        return value;
    }

    RegisterU16& operator+=(u16 num)
    {
        value += num;
        return *this;
    }

    // postfix x++
    RegisterU16 operator++(int)
    {
        RegisterU16 copy(*this);
        operator++();
        return copy;
    }

    // prefix ++x
    RegisterU16& operator++()
    {
        value++;
        return *this;
    }

    RegisterU16& operator-=(u16 num)
    {
        value -= num;
        return *this;
    }

    // postfix x--
    RegisterU16 operator--(int)
    {
        RegisterU16 copy(*this);
        operator--();
        return copy;
    }

    // prefix --x
    RegisterU16& operator--()
    {
        value--;
        return *this;
    }

    bool operator==(u16 num)
    {
        return value == num;
    }

    bool operator==(int num)
    {
        return value == num;
    }

    u8 GetHighByte()
    {
        return (u8)(value >> 8);
    }

    u8 GetLowByte()
    {
        return (u8)(value);
    }

    void SetHighByte(u8 num)
    {
        value &= 0x00FF;
        value += (num << 8);
    }

    void SetLowByte(u8 num)
    {
        value &= 0xFF00;
        value += num;
    }
};