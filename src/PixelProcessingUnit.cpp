#include "PixelProcessingUnit.h"

PixelProcessingUnit::PixelProcessingUnit(Memory* pMemory) 
    : pMemory(pMemory), gpu(pMemory)
{

}

PixelProcessingUnit::~PixelProcessingUnit()
{

}

void PixelProcessingUnit::Write(Address address, Byte value)
{
    mem[address - ADDR_LCDC] = value;

    if (address == ADDR_LCDC)
    {
        if (value & LCD_CTRL_FLAGS::LCD_ON)
        {
            gpu.TurnOnLCD();
            gpu.LoadTileMap(0x3000, 0x7000);
            gpu.Draw();
        }
    }
}

Byte PixelProcessingUnit::Read(Address address)
{
    return mem[address - ADDR_LCDC];
}