#include "ALUHelpers.h"

u16 ALUHelpers::Combine(u8 high, u8 low)
{
	return (high << 8) + low;
}

bool ALUHelpers::Carry7Add(u8 lhs, u8 rhs, u8 carry)
{
	int sum = lhs + rhs + carry;

	return sum > 0xFF;
}

bool ALUHelpers::Carry3Add(u8 lhs, u8 rhs, u8 carry)
{
	return ((lhs & 0xF) + (rhs & 0xF) + carry) > 0xF;
}

bool ALUHelpers::Carry7Sub(u8 lhs, u8 rhs, u8 carry)
{
	int sum = lhs - rhs - carry;

	return sum < 0;
}

bool ALUHelpers::Carry3Sub(u8 lhs, u8 rhs, u8 carry)
{
	return ((lhs & 0xF) - (rhs & 0xF) - carry) < 0;
}

bool ALUHelpers::Carry7Signed(u16 lhs, s8 rhs)
{
	int sum = lhs + rhs;

	if (rhs >= 0)
	{
		return ((lhs & 0xFF) + rhs) > 0xFF;
	}
	else
	{
		return (sum & 0xFF) <= (lhs & 0xFF);
	}
}

bool ALUHelpers::Carry3Signed(u16 lhs, s8 rhs)
{
	int sum = lhs + rhs;

	if (rhs >= 0)
	{
		return ((lhs & 0xF) + (rhs & 0xF)) > 0xF;
	}
	else
	{
		return (sum & 0xF) <= (lhs & 0xF);
	}
}

bool ALUHelpers::Carry15Add(u16 lhs, u16 rhs)
{
	int sum = lhs + rhs;

	return sum > 0xFFFF;
}

bool ALUHelpers::Carry11Add(u16 lhs, u16 rhs)
{
	return (lhs & 0xFff) + (rhs & 0xFff) > 0xFff;
}