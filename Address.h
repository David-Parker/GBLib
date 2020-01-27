#pragma once
#include "GlobalDefinitions.h"

class Address
{
private:
	u16 store;
public:
	static const int ADDRESSSPACE = 65536;
	Address(u16 address) : store(address) {}
	Address() : store(0x0) {}

	operator unsigned short() const
	{
		return store;
	}

	Address operator+=(int num)
	{
		store += num;
		return *this;
	}

	Address operator++(int)
	{
		return operator+=(1);
	}

	~Address();
};

