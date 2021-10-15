#include "PixelProcessingUnit.h"
#include <thread>

PixelProcessingUnit::PixelProcessingUnit(Memory* pMemory, InterruptController* interruptController)
    :   pMemory(pMemory),
        pInterruptController(interruptController),
        backgroundMap(pMemory), 
        clockCycles(0),
        mode(LCD_MODE::OBJ_SEARCH),
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
    this->gManager.Close();
}

Address PixelProcessingUnit::GetBGTileMap()
{
    if (LCDC & BG_CODE_AREA_SELECT)
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

Address PixelProcessingUnit::GetBGTileData()
{
    if (LCDC & BG_CHAR_DATA_SELECT)
    {
        // 0x8000 - 0x87FF
        return 0x8000;
    }
    else
    {
        // 0x8800 - 0x8FFF
        return 0x8800;
    }
}

Address PixelProcessingUnit::GetWindowCodeArea()
{
    if (LCDC & WINDOW_CODE_AREA_SELECT)
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
    if (this->lcdOn)
    {
        return;
    }

    gManager.Init();
    this->lcdOn = true;
}

void PixelProcessingUnit::TurnOffLCD()
{
    if (!this->lcdOn)
    {
        return;
    }

    gManager.Close();
    this->lcdOn = false;
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
    if (!this->lcdOn)
        return;

    for (int i = 0; i < SCREEN_WIDTH; i++)
    {
        Byte color = this->backgroundMap.GetPixel(GetBGTileData(), GetBGTileMap(), (i + SCX) % 256, (LY + SCY) % 256);
        gManager.AddPixel(i, LY, color);
    }
}

void PixelProcessingUnit::Draw()
{
    if (!this->lcdOn)
        return;

    gManager.Clear();
    gManager.Draw();
    gManager.Flush();
}

void PixelProcessingUnit::Tick(u64 cycles)
{
    if (!this->lcdOn)
    {
        return;
    }
    else
    {
        this->clockCycles += cycles;
    }
    
    switch (this->mode)
    {
    case LCD_MODE::OBJ_SEARCH:
        if (this->clockCycles >= CLOCKS_PER_OBJ_SEARCH)
        {
            this->mode = LCD_MODE::VIDEO_READ;
            this->STAT.SetBit(0);
            this->STAT.SetBit(1);
            this->clockCycles -= CLOCKS_PER_OBJ_SEARCH;
        }
        break;
    case LCD_MODE::VIDEO_READ:
        if (this->clockCycles >= CLOCKS_PER_VIDEO_READ)
        {
            this->mode = LCD_MODE::HBLANK;
            this->TestLYCMatch();
            this->STAT.ResetBit(0);
            this->STAT.ResetBit(1);
            this->clockCycles -= CLOCKS_PER_VIDEO_READ;
        }
        break;
    case LCD_MODE::HBLANK:
        if (this->clockCycles >= CLOCKS_PER_HBLANK)
        {
            this->BufferScanLine();
            ++LY;

            if (LY < SCREEN_HEIGHT)
            {
                this->mode = LCD_MODE::OBJ_SEARCH;
                this->STAT.ResetBit(0);
                this->STAT.SetBit(1);
            }
            else
            {
                this->mode = LCD_MODE::VBLANK;
                this->STAT.SetBit(0);
                this->STAT.ResetBit(1);
                this->pInterruptController->RequestInterrupt(INTERRUPT_FLAGS::INT_VBLANK);
            }

            this->clockCycles -= CLOCKS_PER_HBLANK;
        }
        break;
    case LCD_MODE::VBLANK:
        if (this->clockCycles >= CLOCKS_PER_VBLANK)
        {
            ++LY;

            if (LY == 154)
            {
                this->mode = LCD_MODE::OBJ_SEARCH;
                this->STAT.ResetBit(0);
                this->STAT.SetBit(1);
                this->Draw();
                LY = 0;
            }

            this->TestLYCMatch();
            this->gManager.HandleEvents();
            this->clockCycles -= CLOCKS_PER_VBLANK;
        }
        break;
    default:
        throw std::exception("Unknown LCD Mode.");
    }
}

void PixelProcessingUnit::TestLYCMatch()
{
    if (LY == LYC)
    {
        this->STAT.SetBit(LCD_STAT_FLAGS::MATCH_FLAG);
    }
    else
    {
        this->STAT.ResetBit(LCD_STAT_FLAGS::MATCH_FLAG);
    }
}