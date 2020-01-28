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

	operator Address() const
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

	RegisterU16& operator++(int)
	{
		return operator+=(1);
	}

	RegisterU16& operator-=(u16 num)
	{
		value -= num;
		return *this;
	}

	RegisterU16& operator--(int)
	{
		return operator-=(1);
	}

	bool operator==(int num)
	{
		return value == num;
	}
};