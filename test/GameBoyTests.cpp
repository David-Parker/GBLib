#include "CppUnitTest.h"
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

		TEST_METHOD(Register_U8MirrosU16)
		{
			RegisterU16 regU16(42);
			RegisterU8 lower((Byte*)&regU16);

			Assert::IsTrue(regU16 == 42);
			Assert::IsTrue(lower == 42);
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
			Assert::IsTrue(reg == RegisterU8::ZERO_FLAG | RegisterU8::SUB_FLAG);
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

		TEST_METHOD(RegisterU8_Operators)
		{
			Byte value = 0;
			RegisterU8 reg(&value);

			Assert::IsTrue(reg == 0);
			Assert::IsTrue(*reg == 0);

			reg++;
			Assert::IsTrue(reg == 1);
			Assert::IsTrue(*reg == 1);

			reg += 2;
			Assert::IsTrue(reg == 3);
			Assert::IsTrue(*reg == 3);

			reg -= 2;
			Assert::IsTrue(reg == 1);
			Assert::IsTrue(*reg == 1);

			reg--;
			Assert::IsTrue(reg == 0);
			Assert::IsTrue(*reg == 0);
		}

		TEST_METHOD(RegisterU16_Operators)
		{
			RegisterU16 reg;

			Assert::IsTrue(reg == 0);
			Assert::IsTrue(*reg == 0);

			reg++;
			Assert::IsTrue(reg == 1);
			Assert::IsTrue(*reg == 1);

			reg += 2;
			Assert::IsTrue(reg == 3);
			Assert::IsTrue(*reg == 3);

			reg -= 2;
			Assert::IsTrue(reg == 1);
			Assert::IsTrue(*reg == 1);

			reg--;
			Assert::IsTrue(reg == 0);
			Assert::IsTrue(*reg == 0);
		}
	};
}