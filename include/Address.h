#pragma once
#include "GlobalDefinitions.h"

#define ADDR_BOOT_ROM_DMG_START 0x0000
#define ADDR_BOOT_ROM_DMG_END 0x00FF

#define ADDR_BOOT_ROM_CGB_START 0x0000
#define ADDR_BOOT_ROM_CGB_SECOND 0x0200
#define ADDR_BOOT_ROM_CGB_END 0x08FF

#define ADDR_GAME_ROM_START 0x0000
#define ADDR_GAME_ROM_END 0x7FFF

#define ADDR_INTERRUPT_FLAG 0xFF0F
#define ADDR_INTERRUPT_ENABLE 0xFFFF

#define ADDR_SOUND_START 0xFF10
#define ADDR_SOUND_END 0xFF26

#define ADDR_JOYPAD_INPUT 0xFF00

#define ADDR_SERIAL_START 0xFF01
#define ADDR_SERIAL_END 0xFF02
#define ADDR_SERIAL_REG_SB 0xFF01
#define ADDR_SERIAL_REG_SC 0xFF02

#define ADDR_TIMER_START 0xFF04
#define ADDR_TIMER_END 0xFF07
#define ADDR_TIMER_REG_DIV 0xFF04
#define ADDR_TIMER_REG_TIMA 0xFF05
#define ADDR_TIMER_REG_TMA 0xFF06
#define ADDR_TIMER_REG_TAC 0xFF07

#define ADDR_PPU_START 0xFF40
#define ADDR_PPU_END 0xFF7F
#define ADDR_PPU_REG_CONTROL 0xFF40
#define ADDR_PPU_REG_STATUS 0xFF41
#define ADDR_PPU_REG_SCROLL_Y 0xFF42
#define ADDR_PPU_REG_SCROLL_X 0xFF43
#define ADDR_PPU_REG_Y_COORD 0xFF44
#define ADDR_PPU_REG_LY_COMPARE 0xFF45
#define ADDR_PPU_REG_DMA_TRANSFER 0xFF46
#define ADDR_PPU_REG_BG_PALETTE_DATA 0xFF47
#define ADDR_PPU_REG_OBJ_PALETTE_0_DATA 0xFF48
#define ADDR_PPU_REG_OBJ_PALETTE_1_DATA 0xFF49
#define ADDR_PPU_REG_OBJ_WINDOW_Y_POS 0xFF4A
#define ADDR_PPU_REG_OBJ_WINDOW_X_POS_MIN_7 0xFF4B
#define ADDR_PPU_REG_BG_COLOR_PALETTE_INDEX 0xFF68
#define ADDR_PPU_REG_BG_COLOR_PALETTE_DATA 0xFF69
#define ADDR_PPU_REG_OBJ_COLOR_PALETTE_INDEX 0xFF6A
#define ADDR_PPU_REG_OBJ_COLOR_PALETTE_DATA 0xFF6B

#define ADDR_VIDEO_RAM_START 0x8000
#define ADDR_VIDEO_RAM_END 0x9FFF
#define ADDR_VIDEO_RAM_BLOCK_ZERO 0x8000
#define ADDR_VIDEO_RAM_BLOCK_ONE 0x8800
#define ADDR_VIDEO_RAM_BLOCK_TWO 0x9000

#define ADDR_EXTERNAL_RAM_START 0xA000
#define ADDR_EXTERNAL_RAM_END 0xBFFF

#define ADDR_GENERAL_RAM_START 0xC000
#define ADDR_GENERAL_RAM_END 0xDFFF

#define ADDR_OAM_RAM_START 0xFE00
#define ADDR_OAM_RAM_END 0xFE9F

#define ADDR_HIGH_RAM_START 0xFF80
#define ADDR_HIGH_RAM_END 0xFFFF

#define ADDR_ROM_BANK_0_START 0x0000
#define ADDR_ROM_BANK_0_END 0x3FFF
#define ADDR_ROM_BANK_EXTENDABLE_START 0x4000
#define ADDR_ROM_BANK_EXTENDABLE_END 0x7FFF

#define ADDR_MBC_RAM_ENABLE_START 0x0000
#define ADDR_MBC_RAM_ENABLE_END 0x1FFF
#define ADDR_MBC_ROM_BANK_START 0x2000
#define ADDR_MBC_ROM_BANK_END 0x3FFF
#define ADDR_MBC_MODE_SELECT_START 0x6000
#define ADDR_MBC_MODE_SELECT_END 0x7FFF
#define ADDR_MBC_RAM_BANK_START 0x4000
#define ADDR_MBC_RAM_BANK_END 0x5FFF
#define ADDR_MBC3_LATCH_CLOCK_START 0x6000
#define ADDR_MBC3_LATCH_CLOCK_END 0x7FFF

bool AddressInRange(Address addr, Address start, Address end);