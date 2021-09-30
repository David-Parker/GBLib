#include "PixelProcessingUnit.h"

PixelProcessingUnit::PixelProcessingUnit(Memory* pMemory) 
    : pMemory(pMemory)
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
            this->LoadTilePatternTable(GetBGCharArea());
            this->LoadTileMap(GetBGCodeArea());
            this->Draw();
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

void PixelProcessingUnit::LoadTilePatternTable(Address start)
{
    if (start == 0x8800)
    {
        throw new std::exception("Secondary tile map is not implemented.");
    }

    Byte buf[16];

    for (int i = 0; i < 256; i++)
    {
        // Read in the next tile (16 bytes)
        for (int j = 0; j < 16; j++)
        {
            buf[j] = pMemory->Read(start++);
        }

        Tile& tile = this->tilePatternTable[i];
        tile.LoadData(buf);
    }
}

void PixelProcessingUnit::LoadTileMap(Address start)
{
    int x = 0;

    for (int i = 0; i < sizeY; ++i)
    {
        for (int j = 0; j < sizeX; j++)
        {
            tileMap[i][j] = pMemory->Read(start++);
        }
    }
}

void PixelProcessingUnit::LoadColorPalette()
{
    Byte bgPalette = pMemory->Read(ADDR_PPU_REG_BG_PALETTE_DATA);

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

void PixelProcessingUnit::Draw()
{
    // Get Current tiles pointed by memory.

    for (int i = 0; i < TILE_HEIGHT; i++)
    {
        for (int j = 0; j < TILE_WIDTH; j++)
        {
            for (int k = 0; k < 8; k++)
            {
                for (int l = 0; l < 8; l++)
                {
                    Byte tileIdx = this->tileMap[i][j];
                    Tile& tile = this->tilePatternTable[tileIdx];
                    gManager.AddPixel((j * 8) + l, (i * 8) + k, tile.GetPixel(l, k));
                }
            }
        }
    }

    gManager.Clear();
    gManager.Draw();
    gManager.Flush();
}