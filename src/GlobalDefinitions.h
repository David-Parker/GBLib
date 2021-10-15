#pragma once
#include <stdint.h>
#include <exception>

typedef uint8_t u8;
typedef int8_t s8;
typedef uint16_t u16;
typedef int16_t s16;
typedef uint32_t u32;
typedef int32_t s32;
typedef uint64_t u64;
typedef int64_t s64;

typedef u8 Byte;

constexpr static int ROM_SIZE = 0x8000;
constexpr static int CLOCK_HZ = 4194304;
constexpr static int CLOCK_NS_PER_CYCLE = 238;
constexpr static int CLOCK_CYCLES_PER_MACHINE_CYCLE = 4;
constexpr static int CLOCKS_PER_OBJ_SEARCH = 80;
constexpr static int CLOCKS_PER_VIDEO_READ = 172;
constexpr static int CLOCKS_PER_HBLANK = 204;
constexpr static int CLOCKS_PER_VBLANK = 456;
constexpr static int CLOCKS_PER_DIVIDER_INC = 64;
constexpr static int SCALE = 3;
constexpr static int SCREEN_WIDTH = 160;
constexpr static int SCREEN_HEIGHT = 144;
constexpr static int SDL_SCREEN_WIDTH = SCREEN_WIDTH * SCALE;
constexpr static int SDL_SCREEN_HEIGHT = SCREEN_HEIGHT * SCALE;
constexpr static int INVALID_READ = 0xFF;