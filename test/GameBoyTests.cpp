#include "ALUHelpers.h"
#include "CppUnitTest.h"
#include "Cpu.h"
#include "RegisterU16.h"
#include "RegisterU8.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace GameBoyTests
{
    TEST_CLASS(UnitTest1)
    {
    public:

        TEST_METHOD(Register_CreateU8FromU16)
        {
            RegisterU16 regU16;
            RegisterU8 upper((Byte*)&regU16 + 1);
            RegisterU8 lower((Byte*)&regU16);

            upper += 1;

            Assert::IsTrue(upper == 1);
            Assert::IsTrue(lower == 0);
            Assert::IsTrue(regU16 == 0b0000000100000000);

            lower += 1;

            Assert::IsTrue(upper == 1);
            Assert::IsTrue(lower == 1);
            Assert::IsTrue(regU16 == 0b0000000100000001);

            upper = 0;
            lower = 0;

            Assert::IsTrue(upper == 0);
            Assert::IsTrue(lower == 0);
            Assert::IsTrue(regU16 == 0);
        }

        TEST_METHOD(Register_U8MirrorsU16)
        {
            RegisterU16 regU16(42);
            RegisterU8 lower((Byte*)&regU16);

            Assert::IsTrue(regU16 == 42);
            Assert::IsTrue(lower == 42);
        }

        TEST_METHOD(Register_U8MirrorsU16AfterIncrement)
        {
            RegisterU16 regU16;
            RegisterU8 upper((Byte*)&regU16 + 1);
            RegisterU8 lower((Byte*)&regU16);

            ++regU16;

            Assert::IsTrue(upper == 0);
            Assert::IsTrue(lower == 1);
            Assert::IsTrue(regU16 == 1);

            RegisterU16 prev = regU16++;

            Assert::IsTrue(upper == 0);
            Assert::IsTrue(lower == 2);
            Assert::IsTrue(regU16 == 2);
            Assert::IsTrue(prev == 1);
        }

        TEST_METHOD(Register_SetFlags)
        {
            Byte value = 0;
            RegisterU8 reg(&value);

            Assert::IsTrue(reg == 0);

            reg.SetFlags(RegisterU8::ZERO_FLAG);
            Assert::IsTrue(reg == RegisterU8::ZERO_FLAG);
        }

        TEST_METHOD(Register_SetFlags_Multi)
        {
            Byte value = 0;
            RegisterU8 reg(&value);

            Assert::IsTrue(reg == 0);

            reg.SetFlags(RegisterU8::ZERO_FLAG | RegisterU8::SUB_FLAG);
            Assert::IsTrue(reg == (RegisterU8::ZERO_FLAG | RegisterU8::SUB_FLAG));
        }

        TEST_METHOD(Register_ClearFlags)
        {
            Byte value = 0;
            RegisterU8 reg(&value);

            Assert::IsTrue(reg == 0);

            reg.SetFlags(RegisterU8::ZERO_FLAG);
            reg.ClearFlags(RegisterU8::ZERO_FLAG);
            Assert::IsTrue(reg == 0);
        }

        TEST_METHOD(Register_ClearFlags_Multi)
        {
            Byte value = 0;
            RegisterU8 reg(&value);

            Assert::IsTrue(reg == 0);

            reg.SetFlags(RegisterU8::ZERO_FLAG | RegisterU8::SUB_FLAG);
            reg.ClearFlags(RegisterU8::ZERO_FLAG);
            Assert::IsTrue(reg == RegisterU8::SUB_FLAG);

            reg.ClearFlags(RegisterU8::SUB_FLAG);
            Assert::IsTrue(reg == 0);
        }

        TEST_METHOD(Register_FlagIsSet)
        {
            Byte value = 0;
            RegisterU8 reg(&value);

            Assert::IsTrue(reg == 0);

            Assert::IsFalse(reg.FlagIsSet(RegisterU8::ZERO_FLAG));
            Assert::IsFalse(reg.FlagIsSet(RegisterU8::CARRY_FLAG));
            Assert::IsFalse(reg.FlagIsSet(RegisterU8::HCARRY_FLAG));
            Assert::IsFalse(reg.FlagIsSet(RegisterU8::SUB_FLAG));

            reg.SetFlags(RegisterU8::ZERO_FLAG);
            Assert::IsTrue(reg.FlagIsSet(RegisterU8::ZERO_FLAG));
            Assert::IsFalse(reg.FlagIsSet(RegisterU8::CARRY_FLAG));
            Assert::IsFalse(reg.FlagIsSet(RegisterU8::HCARRY_FLAG));
            Assert::IsFalse(reg.FlagIsSet(RegisterU8::SUB_FLAG));

            reg.SetFlags(RegisterU8::CARRY_FLAG);
            Assert::IsTrue(reg.FlagIsSet(RegisterU8::ZERO_FLAG));
            Assert::IsTrue(reg.FlagIsSet(RegisterU8::CARRY_FLAG));
            Assert::IsFalse(reg.FlagIsSet(RegisterU8::HCARRY_FLAG));
            Assert::IsFalse(reg.FlagIsSet(RegisterU8::SUB_FLAG));

            reg.SetFlags(RegisterU8::HCARRY_FLAG);
            Assert::IsTrue(reg.FlagIsSet(RegisterU8::ZERO_FLAG));
            Assert::IsTrue(reg.FlagIsSet(RegisterU8::CARRY_FLAG));
            Assert::IsTrue(reg.FlagIsSet(RegisterU8::HCARRY_FLAG));
            Assert::IsFalse(reg.FlagIsSet(RegisterU8::SUB_FLAG));

            reg.SetFlags(RegisterU8::SUB_FLAG);
            Assert::IsTrue(reg.FlagIsSet(RegisterU8::ZERO_FLAG));
            Assert::IsTrue(reg.FlagIsSet(RegisterU8::CARRY_FLAG));
            Assert::IsTrue(reg.FlagIsSet(RegisterU8::HCARRY_FLAG));
            Assert::IsTrue(reg.FlagIsSet(RegisterU8::SUB_FLAG));

            reg.ClearAllFlags();
            Assert::IsFalse(reg.FlagIsSet(RegisterU8::ZERO_FLAG));
            Assert::IsFalse(reg.FlagIsSet(RegisterU8::CARRY_FLAG));
            Assert::IsFalse(reg.FlagIsSet(RegisterU8::HCARRY_FLAG));
            Assert::IsFalse(reg.FlagIsSet(RegisterU8::SUB_FLAG));
        }

        TEST_METHOD(RegisterU8_Operators)
        {
            Byte value = 0;
            RegisterU8 reg(&value);
            RegisterU8 res(&value);

            Assert::IsTrue(reg == 0);
            Assert::IsTrue(*reg == 0);

            res = ++reg;
            Assert::IsTrue(reg == 1);
            Assert::IsTrue(res == 1);
            Assert::IsTrue(*reg == 1);

            res = reg += 2;
            Assert::IsTrue(reg == 3);
            Assert::IsTrue(res == 3);
            Assert::IsTrue(*reg == 3);

            res = reg -= 2;
            Assert::IsTrue(reg == 1);
            Assert::IsTrue(res == 1);
            Assert::IsTrue(*reg == 1);

            res = --reg;
            Assert::IsTrue(reg == 0);
            Assert::IsTrue(res == 0);
            Assert::IsTrue(*reg == 0);
        }

        TEST_METHOD(RegisterU16_Operators)
        {
            RegisterU16 reg;
            RegisterU16 res;

            Assert::IsTrue(reg == 0);
            Assert::IsTrue(*reg == 0);

            res = reg++;
            Assert::IsTrue(reg == 1);
            Assert::IsTrue(res == 0);
            Assert::IsTrue(*reg == 1);

            res = reg += 2;
            Assert::IsTrue(reg == 3);
            Assert::IsTrue(res == 3);
            Assert::IsTrue(*reg == 3);

            res = reg -= 2;
            Assert::IsTrue(reg == 1);
            Assert::IsTrue(res == 1);
            Assert::IsTrue(*reg == 1);

            res = reg--;
            Assert::IsTrue(reg == 0);
            Assert::IsTrue(res == 1);
            Assert::IsTrue(*reg == 0);
        }

        TEST_METHOD(RegisterU16_GetHighByte)
        {
            RegisterU16 reg = 0b1001100100000000;
            u8 high = reg.GetHighByte();
            Assert::IsTrue(high == 0b10011001);
        }

        TEST_METHOD(RegisterU16_GetLowByte)
        {
            RegisterU16 reg = 0b0000000010011001;
            u8 low = reg.GetLowByte();
            Assert::IsTrue(low == 0b10011001);
        }

        TEST_METHOD(RegisterU16_SetHighByte)
        {
            RegisterU16 reg = 0b1111111110011001;
            reg.SetHighByte(0b10011001);
            Assert::IsTrue(reg == 0b1001100110011001);
        }

        TEST_METHOD(RegisterU16_SetLowByte)
        {
            RegisterU16 reg = 0b1001100111111111;
            reg.SetLowByte(0b10011001);
            Assert::IsTrue(reg == 0b1001100110011001);
        }

        TEST_METHOD(General_UnsignedRightShift)
        {
            u8 value = 0b10000000;
            int bit7 = (value & 0b10000000) >> 7;
            Assert::IsTrue(bit7 == 1);
        }

        TEST_METHOD(General_UnsignedLeftShift)
        {
            u8 value = 0b00000001;
            int bit0 = value << 7;
            Assert::IsTrue(bit0 == 0b10000000);
        }

        TEST_METHOD(RegisterU8_Operator_Equals)
        {
            Byte value1 = 0xFF;
            Byte value2 = 0xFF;
            RegisterU8 reg1(&value1);
            RegisterU8 reg2(&value2);
            Assert::IsTrue(value1 == 0xFF);
            Assert::IsTrue(value1 == value2);
        }

        TEST_METHOD(RegisterU8_Operator_Not_Equals)
        {
            Byte value1 = 0xFF;
            Byte value2 = 0xFE;
            RegisterU8 reg1(&value1);
            RegisterU8 reg2(&value2);
            Assert::IsTrue(value1 != value2);
        }

        TEST_METHOD(RegisterU8_GetHighNibble)
        {
            Byte value = 0b11010000;
            RegisterU8 reg(&value);
            u8 high = reg.GetHighNibble();
            Assert::IsTrue(high == 0b00001101);
        }

        TEST_METHOD(RegisterU8_GetLowNibble)
        {
            Byte value = 0b00001101;
            RegisterU8 reg(&value);
            u8 low = reg.GetLowNibble();
            Assert::IsTrue(low == 0b00001101);
        }

        TEST_METHOD(RegisterU8_SetHighNibble)
        {
            Byte value = 0b00001011;
            RegisterU8 reg(&value);
            reg.SetHighNibble(0b00001101);
            Assert::IsTrue(value == 0b11011011);
        }

        TEST_METHOD(RegisterU8_SetLowNibble)
        {
            Byte value = 0b10110000;
            RegisterU8 reg(&value);
            reg.SetLowNibble(0b00001101);
            Assert::IsTrue(value == 0b10111101);
        }

        TEST_METHOD(RegisterU8_GetBit)
        {
            Byte value = 0b10101010;
            RegisterU8 reg(&value);

            Assert::IsTrue(reg.GetBit(0) == 0);
            Assert::IsTrue(reg.GetBit(1) == 1);
            Assert::IsTrue(reg.GetBit(2) == 0);
            Assert::IsTrue(reg.GetBit(3) == 1);
            Assert::IsTrue(reg.GetBit(4) == 0);
            Assert::IsTrue(reg.GetBit(5) == 1);
            Assert::IsTrue(reg.GetBit(6) == 0);
            Assert::IsTrue(reg.GetBit(7) == 1);
        }

        TEST_METHOD(RegisterU8_SetBit)
        {
            Byte value = 0;
            RegisterU8 reg(&value);

            reg.SetBit(1);
            reg.SetBit(3);
            reg.SetBit(5);
            reg.SetBit(7);

            Assert::IsTrue(reg.GetBit(0) == 0);
            Assert::IsTrue(reg.GetBit(1) == 1);
            Assert::IsTrue(reg.GetBit(2) == 0);
            Assert::IsTrue(reg.GetBit(3) == 1);
            Assert::IsTrue(reg.GetBit(4) == 0);
            Assert::IsTrue(reg.GetBit(5) == 1);
            Assert::IsTrue(reg.GetBit(6) == 0);
            Assert::IsTrue(reg.GetBit(7) == 1);
        }

        TEST_METHOD(RegisterU8_ResetBit)
        {
            Byte value = 0;
            RegisterU8 reg(&value);

            reg.SetBit(1);
            reg.SetBit(3);
            reg.SetBit(5);
            reg.SetBit(7);

            Assert::IsTrue(reg.GetBit(0) == 0);
            Assert::IsTrue(reg.GetBit(1) == 1);
            Assert::IsTrue(reg.GetBit(2) == 0);
            Assert::IsTrue(reg.GetBit(3) == 1);
            Assert::IsTrue(reg.GetBit(4) == 0);
            Assert::IsTrue(reg.GetBit(5) == 1);
            Assert::IsTrue(reg.GetBit(6) == 0);
            Assert::IsTrue(reg.GetBit(7) == 1);

            reg.ResetBit(1);
            reg.ResetBit(3);
            reg.ResetBit(5);
            reg.ResetBit(7);

            Assert::IsTrue(reg.GetBit(0) == 0);
            Assert::IsTrue(reg.GetBit(1) == 0);
            Assert::IsTrue(reg.GetBit(2) == 0);
            Assert::IsTrue(reg.GetBit(3) == 0);
            Assert::IsTrue(reg.GetBit(4) == 0);
            Assert::IsTrue(reg.GetBit(5) == 0);
            Assert::IsTrue(reg.GetBit(6) == 0);
            Assert::IsTrue(reg.GetBit(7) == 0);
        }

        TEST_METHOD(ALUHelpers_Combine)
        {
            u8 one = 0;
            u8 two = 0;

            u16 result = ALUHelpers::Combine(one, two);

            Assert::IsTrue(result == 0);

            one = 1;
            two = 1;

            result = ALUHelpers::Combine(one, two);

            Assert::IsTrue(result == 0x0101);
        }

        TEST_METHOD(ALUHelpers_CarryU8)
        {
            Assert::IsFalse(ALUHelpers::Carry7Add(0, 0, 0));
            Assert::IsFalse(ALUHelpers::Carry7Add(1, 1, 0));
            Assert::IsFalse(ALUHelpers::Carry7Add(127, 128, 0));
            Assert::IsTrue(ALUHelpers::Carry7Add(128, 128, 0));
            Assert::IsTrue(ALUHelpers::Carry7Add(255, 255, 0));
        }

        TEST_METHOD(ALUHelpers_HCarryU8)
        {
            Assert::IsFalse(ALUHelpers::Carry3Add(0, 0, 0));
            Assert::IsFalse(ALUHelpers::Carry3Add(1, 1, 0));
            Assert::IsFalse(ALUHelpers::Carry3Add(7, 8, 0));
            Assert::IsTrue(ALUHelpers::Carry3Add(8, 8, 0));
            Assert::IsTrue(ALUHelpers::Carry3Add(255, 1, 0));
            Assert::IsTrue(ALUHelpers::Carry3Add(255, 127, 0));
            Assert::IsFalse(ALUHelpers::Carry3Add(255, 128, 0));
        }
    };
}