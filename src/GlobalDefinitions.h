#pragma once
#include <stdint.h>

typedef uint8_t u8;
typedef int8_t s8;
typedef uint16_t u16;
typedef int16_t s16;
typedef uint32_t u32;
typedef int32_t s32;
typedef uint64_t u64;
typedef int64_t s64;

typedef u8 Byte;

constexpr static int CLOCK_HZ = 4194304;
constexpr static int CLOCK_NS_PER_CYCLE = 238;
constexpr static int CLOCK_CYCLES_PER_MACHINE_CYCLE = 4;
constexpr static int CLOCKS_PER_SCANLINE = 252;
constexpr static int SCALE = 3;
constexpr static int TILEMAP_SIZE = 32;
constexpr static int SCREEN_WIDTH = 160;
constexpr static int SCREEN_HEIGHT = 144;
constexpr static int SDL_SCREEN_WIDTH = SCREEN_WIDTH * SCALE;
constexpr static int SDL_SCREEN_HEIGHT = SCREEN_HEIGHT * SCALE;