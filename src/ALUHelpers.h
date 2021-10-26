#pragma once
#include "GlobalDefinitions.h"

namespace ALUHelpers
{
    u16 GAMEBOY_API Combine(u8 high, u8 low);
    bool GAMEBOY_API Carry7Add(u8 lhs, u8 rhs, u8 carry);
    bool GAMEBOY_API Carry3Add(u8 lhs, u8 rhs, u8 carry);
    bool GAMEBOY_API Carry7Sub(u8 lhs, u8 rhs, u8 carry);
    bool GAMEBOY_API Carry3Sub(u8 lhs, u8 rhs, u8 carry);
    bool GAMEBOY_API Carry7Signed(u16 lhs, s8 rhs);
    bool GAMEBOY_API Carry3Signed(u16 lhs, s8 rhs);
    bool GAMEBOY_API Carry15Add(u16 lhs, u16 rhs);
    bool GAMEBOY_API Carry11Add(u16 lhs, u16 rhs);
}