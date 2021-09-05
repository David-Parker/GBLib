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

const static int SCALE = 4;
const static int TILE_WIDTH = 18;
const static int TILE_HEIGHT = 20;
const static int SCREEN_WIDTH = TILE_WIDTH * 8 * SCALE;
const static int SCREEN_HEIGHT = TILE_HEIGHT * 8 * SCALE;