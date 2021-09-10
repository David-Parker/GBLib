#pragma once
#include "GlobalDefinitions.h"
#include "RegisterU16.h"

class RegisterU8
{
private:
    u8* pValue;

public:
    enum Flags
    {
        ZERO_FLAG = 0b10000000,
        SUB_FLAG = 0b01000000,
        HCARRY_FLAG = 0b00100000,
        CARRY_FLAG = 0b00010000
    };

    RegisterU8(u8* pValue) : pValue(pValue) {}
    RegisterU8(RegisterU8& rhs) : pValue(rhs.pValue) {}
    ~RegisterU8() {}

    operator u8() const
    {
        return *pValue;
    }

    int operator=(int num)
    {
        *pValue = num;
        return *pValue;
    }

    int operator=(RegisterU8& other)
    {
        *pValue = *other;
        return *pValue;
    }

    u8 operator*()
    {
        return *pValue;
    }

    RegisterU8& operator+=(u8 num)
    {
        *pValue += num;
        return *this;
    }

    // prefix ++x
    RegisterU8& operator++()
    {
        *pValue += 1;
        return *this;
    }

    RegisterU8& operator-=(u8 num)
    {
        *pValue -= num;
        return *this;
    }

    // prefix --x
    RegisterU8& operator--()
    {
        *pValue -= 1;
        return *this;
    }

    bool operator==(int num)
    {
        return *pValue == num;
    }

    void SetFlags(u8 flags)
    {
        *pValue = (*pValue | flags);
    }

    void ClearFlags(u8 flags)
    {
        *pValue = (*pValue & ~flags);
    }

    bool FlagIsSet(u8 flags)
    {
        if ((flags & *pValue) > 0)
        {
            return true;
        }
        else
        {
            return false;
        }
    }

    void ClearAllFlags()
    {
        *pValue = 0;
    }

    u8 GetHighNibble()
    {
        return *pValue >> 4;
    }

    u8 GetLowNibble()
    {
        return *pValue & 0b00001111;
    }

    void SetHighNibble(u8 num)
    {
        *pValue &= 0x0F;
        *pValue += (num << 4);
    }

    void SetLowNibble(u8 num)
    {
        *pValue &= 0xF0;
        *pValue += (num & 0x0F);
    }

    int GetBit(int bit)
    {
        return (*pValue >> bit) & 0b00000001;
    }

    void SetBit(int bit)
    {
        *pValue |= (1 << bit);
    }

    void ResetBit(int bit)
    {
        *pValue &= ~(1 << bit);
    }
};