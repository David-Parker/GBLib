#pragma once
#include <stdint.h>
#include <exception>

#ifdef GAMEBOY_EXPORTING
#define GAMEBOY_API __declspec(dllexport)
#else
#define GAMEBOY_API __declspec(dllimport)
#endif

typedef uint8_t u8;
typedef int8_t s8;
typedef uint16_t u16;
typedef int16_t s16;
typedef uint32_t u32;
typedef int32_t s32;
typedef uint64_t u64;
typedef int64_t s64;

typedef u8 Byte;
typedef u16 Address;

constexpr static int ADDRESSSPACE = 65536;
constexpr static int CLOCK_HZ = 4194304;
constexpr static int CLOCK_NS_PER_CYCLE = 238;
constexpr static int CLOCK_NS_PER_FRAME = 16637390;
constexpr static int CLOCK_CYCLES_PER_FRAME = CLOCK_HZ / 60;
constexpr static int CLOCK_CYCLES_PER_MACHINE_CYCLE = 4;
constexpr static int CLOCKS_PER_OAM_SEARCH = 80;
constexpr static int CLOCKS_PER_VIDEO_READ = 172;
constexpr static int CLOCKS_PER_HBLANK = 204;
constexpr static int CLOCKS_PER_VBLANK = 456;
constexpr static int CLOCKS_PER_DIVIDER_INC = 64;
constexpr static int SCALE = 4;
constexpr static int SCREEN_WIDTH = 160;
constexpr static int SCREEN_HEIGHT = 144;
constexpr static int SCALED_SCREEN_WIDTH = SCREEN_WIDTH * SCALE;
constexpr static int SCALED_SCREEN_HEIGHT = SCREEN_HEIGHT * SCALE;
constexpr static int SPRITE_NUM_OBJS = 40;
constexpr static int SPRITE_OBJ_LIMIT = 10;
constexpr static int INVALID_READ = 0xFF;
constexpr static int ROM_BANK_BYTES = 16384;
constexpr static int RAM_BANK_BYTES = 8192;

#ifdef _DEBUG
constexpr static int SPEED_MULTIPLIER = 16;
#else
constexpr static int SPEED_MULTIPLIER = 1;
#endif