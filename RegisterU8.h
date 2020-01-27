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
	~RegisterU8() {}

	operator u8() const
	{
		return *pValue;
	}

	int operator=(int num)
	{
		*pValue = num;
		return num;
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

	RegisterU8& operator++(int)
	{
		return operator+=(1);
	}

	RegisterU8& operator-=(u8 num)
	{
		*pValue -= num;
		return *this;
	}

	RegisterU8& operator--(int)
	{
		return operator-=(1);
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

	void ClearAllFlags()
	{
		*pValue = 0;
	}
};