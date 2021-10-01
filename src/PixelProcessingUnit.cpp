#include "PixelProcessingUnit.h"
#include <thread>

PixelProcessingUnit::PixelProcessingUnit(Memory* pMemory) 
    :   pMemory(pMemory),
        backgroundMap(pMemory), 
        LCDC(&mem[ADDR_PPU_REG_CONTROL - ADDR_PPU_START]),
        STAT(&mem[ADDR_PPU_REG_STATUS - ADDR_PPU_START]),
        SCY(&mem[ADDR_PPU_REG_SCROLL_Y - ADDR_PPU_START]),
        SCX(&mem[ADDR_PPU_REG_SCROLL_X - ADDR_PPU_START]),
        LY(&mem[ADDR_PPU_REG_Y_COORD - ADDR_PPU_START]),
        LYC(&mem[ADDR_PPU_REG_LY_COMPARE - ADDR_PPU_START]),
        DMA(&mem[ADDR_PPU_REG_DMA_TRANSFER - ADDR_PPU_START]),
        BGP(&mem[ADDR_PPU_REG_BG_PALETTE_DATA - ADDR_PPU_START]),
        OBP0(&mem[ADDR_PPU_REG_OBJ_PALETTE_0_DATA - ADDR_PPU_START]),
        OBP1(&mem[ADDR_PPU_REG_OBJ_PALETTE_1_DATA - ADDR_PPU_START]),
        WY(&mem[ADDR_PPU_REG_OBJ_WINDOW_Y_POS - ADDR_PPU_START]),
        WX(&mem[ADDR_PPU_REG_OBJ_WINDOW_X_POS_MIN_7 - ADDR_PPU_START])
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
            this->TurnOnLCD();
            this->backgroundMap.LoadTilePatternTable(GetBGCharArea());
            this->backgroundMap.LoadTileMap(GetBGCodeArea());
            //this->Draw();
        }
    }
    else if (address == ADDR_PPU_REG_BG_PALETTE_DATA)
    {
        this->LoadColorPalette();
    }
}

Byte PixelProcessingUnit::Read(Address address)
{
    return mem[address - ADDR_PPU_START];
}

void PixelProcessingUnit::TurnOnLCD()
{
    gManager.Init();
}

void PixelProcessingUnit::LoadColorPalette()
{
    Byte bgPalette = BGP;

    for (int i = 0; i < 4; ++i)
    {
        Byte code = bgPalette & 0b00000011;
        bgPalette >>= 2;

        switch (code)
        {
        case 0:
            gManager.SetPalette(i, DMG_COLOR_WHITE);
            break;
        case 1:
            gManager.SetPalette(i, DMG_COLOR_LIGHT_GRAY);
            break;
        case 2:
            gManager.SetPalette(i, DMG_COLOR_DARK_GRAY);
            break;
        case 3:
            gManager.SetPalette(i, DMG_COLOR_BLACK);
            break;
        default:
            throw std::exception("Invalid color code.");
        }
    }

    gManager.ReloadPalette();
}

bool PixelProcessingUnit::LCDIsOn()
{
    return LCDC & LCD_CTRL_FLAGS::LCD_ON;
}

void PixelProcessingUnit::BufferScanLine()
{
    for (int j = 0; j < SCREEN_WIDTH; j++)
    {
        Byte color = this->backgroundMap.GetPixel((j + SCX) % 256, (LY + SCY) % 256);
        gManager.AddPixel(j, LY, color);
    }
}

void PixelProcessingUnit::Draw()
{
    gManager.Clear();
    gManager.Draw();
    gManager.Flush();
}

void PixelProcessingUnit::Tick(int cycles)
{
    this->clockCycles += cycles;

    if (!this->LCDIsOn())
    {
        return;
    }

    if (this->clockCycles >= CLOCKS_PER_SCANLINE)
    {
        this->clockCycles %= CLOCKS_PER_SCANLINE;

        if (LY < 144)
        {
            this->BufferScanLine();
        }
        else if (LY == 153)
        {
            this->Draw();
        }

        ++LY;
        LY = LY % 154;
    }
}