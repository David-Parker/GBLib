#pragma once
#include "GlobalDefinitions.h"

namespace ALUHelpers
{
	u16 Combine(u8 high, u8 low);
	bool Carry7Add(u8 lhs, u8 rhs, u8 carry);
	bool Carry3Add(u8 lhs, u8 rhs, u8 carry);
	bool Carry7Sub(u8 lhs, u8 rhs, u8 carry);
	bool Carry3Sub(u8 lhs, u8 rhs, u8 carry);
	bool Carry7Signed(u16 lhs, s8 rhs);
	bool Carry3Signed(u16 lhs, s8 rhs);
	bool Carry15Add(u16 lhs, u16 rhs);
	bool Carry11Add(u16 lhs, u16 rhs);
}