#include "PixelProcessingUnit.h"

PixelProcessingUnit::PixelProcessingUnit(Memory* pMemory) 
    : pMemory(pMemory), gpu(pMemory)
{

}

PixelProcessingUnit::~PixelProcessingUnit()
{

}

Address PixelProcessingUnit::GetBGCodeArea()
{
    Byte reg = mem[ADDR_PPU_REG_CONTROL - ADDR_PPU_START];

    if (reg & BG_CODE_AREA_SELECT)
    {
        // 0x9C00 - 0x9FFF
        return 0x9C00;
    }
    else
    {
        // 0x9800 - 0x9BFF
        return 0x9800;
    }
}

Address PixelProcessingUnit::GetBGCharArea()
{
    Byte reg = mem[ADDR_PPU_REG_CONTROL - ADDR_PPU_START];

    if (reg & BG_CHAR_DATA_SELECT)
    {
        // 0x8000 - 0x8FFF
        return 0x8000;
    }
    else
    {
        // 0x8800 - 0x97FF
        return 0x8800;
    }
}

Address PixelProcessingUnit::GetWindowCodeArea()
{
    Byte reg = mem[ADDR_PPU_REG_CONTROL - ADDR_PPU_START];

    if (reg & WINDOW_CODE_AREA_SELECT)
    {
        // 0x9C00 - 0x9FFF
        return 0x9C00;
    }
    else
    {
        // 0x9800 - 0x9BFF
        return 0x9800;
    }
}

void PixelProcessingUnit::Write(Address address, Byte value)
{
    mem[address - ADDR_PPU_START] = value;

    if (address == ADDR_PPU_REG_CONTROL)
    {
        if (value & LCD_CTRL_FLAGS::LCD_ON)
        {
            gpu.TurnOnLCD();
            gpu.LoadTilePatternTable(GetBGCharArea());
            gpu.LoadTileMap(GetBGCodeArea());
            gpu.Draw();
        }
    }
    else if (address == ADDR_PPU_REG_BG_PALETTE_DATA)
    {
        gpu.LoadColorPalette();
    }
}

Byte PixelProcessingUnit::Read(Address address)
{
    return mem[address - ADDR_PPU_START];
}