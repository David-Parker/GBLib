#include "Gpu.h"

void Gpu::TurnOnLCD()
{
    gManager.Init();
}

void Gpu::LoadTilePatternTable(Address start)
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

void Gpu::LoadTileMap(Address start)
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

void Gpu::LoadColorPalette()
{
    Byte codes[4];

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

void Gpu::Draw()
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