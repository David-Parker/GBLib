#pragma once
#include <string>

class Byte
{
private:
	unsigned char store;
public:
	Byte() : store(0) {}

	operator char() const
	{
		return store;
	}

	Byte operator+=(int num)
	{
		store += num;
		return *this;
	}

	Byte operator++(int)
	{
		return operator+=(1);
	}

	Byte(unsigned char str) : store(str) {}
	~Byte();
	std::string ToString();
};

