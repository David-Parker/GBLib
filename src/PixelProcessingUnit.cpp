#include "PixelProcessingUnit.h"
#include <thread>

PixelProcessingUnit::PixelProcessingUnit(Memory* pMemory, InterruptController* interruptController, JoypadController* joypadController)
    :   pMemory(pMemory),
        pInterruptController(interruptController),
        backgroundMap(pMemory), 
        clockCycles(0),
        mode(LCD_MODE::OBJ_SEARCH),
        lcdInitialized(false),
        gManager(SDL_SCREEN_WIDTH, SDL_SCREEN_HEIGHT, SCALE, 3),
        iManager(pMemory, interruptController, joypadController),
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
#ifdef _DEBUG
    this->tileDebugger = new GraphicsManager(192 * 3, 128 * 3, 3, 1);
    this->tileDebugger->Init();
#endif
}

PixelProcessingUnit::~PixelProcessingUnit()
{
}

Address PixelProcessingUnit::GetBGTileMap()
{
    if (LCDC.FlagIsSet(BG_CODE_AREA_SELECT))
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
    if (LCDC.FlagIsSet(BG_CHAR_DATA_SELECT))
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
    if (LCDC.FlagIsSet(WINDOW_CODE_AREA_SELECT))
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
    if (address == ADDR_PPU_REG_CONTROL)
    {
        LCDC = value;

        if (LCDC.FlagIsSet(LCD_CTRL_FLAGS::LCD_ON))
        {
            this->TurnOnLCD();
        }
    }
    else if (address == ADDR_PPU_REG_STATUS)
    {
        Byte upper = 0b01111000 & value;
        Byte lower = 0b00000111 & STAT;
        
        STAT = upper + lower;
    }
    else if (address == ADDR_PPU_REG_Y_COORD)
    {
        return;
    }
    else if (address == ADDR_PPU_REG_DMA_TRANSFER)
    {
        Address source = value * 0x100;
        Address dest = ADDR_OAM_RAM_START;

        for (int i = 0; i <= 0x9F; ++i)
        {
            pMemory->Write(dest+i, pMemory->Read(source+i));
        }
    }
    else if (address == ADDR_PPU_REG_BG_PALETTE_DATA)
    {
        BGP = value;
        LoadColorPalette(BGP, this->bgPalette);
    }
    else if (address == ADDR_PPU_REG_OBJ_PALETTE_0_DATA)
    {
        OBP0 = value;
        LoadColorPalette(OBP0, this->objPalette0);
    }
    else if (address == ADDR_PPU_REG_OBJ_PALETTE_1_DATA)
    {
        OBP1 = value;
        LoadColorPalette(OBP1, this->objPalette1);
    }
    else
    {
        mem[address - ADDR_PPU_START] = value;
    }
}

Byte PixelProcessingUnit::Read(Address address)
{
    return mem[address - ADDR_PPU_START];
}

void PixelProcessingUnit::TurnOnLCD()
{
    if (!this->lcdInitialized)
    {
        this->clockCycles = 0;
        this->mode = LCD_MODE::OBJ_SEARCH;
        LY = 0;
        this->TestLYCMatch();
        gManager.Init();
        this->lcdInitialized = true;
    }
}

void PixelProcessingUnit::TurnOffLCD()
{
    // gManager.Close();
}

bool PixelProcessingUnit::LCDIsOn()
{
    return LCDC.FlagIsSet(LCD_CTRL_FLAGS::LCD_ON);
}

void PixelProcessingUnit::BufferScanLine()
{
    if (!this->LCDIsOn())
    {
        return;
    }

    // BG
    for (int i = 0; i < SCREEN_WIDTH; i++)
    {
        Byte color = this->backgroundMap.GetPixel(GetBGTileData(), GetBGTileMap(), (i + SCX) % 256, (LY + SCY) % 256);
        gManager.AddPixel(i, LY, color, this->bgPalette, LCD_LAYER_BG, false);
    }

    // Window
}

void PixelProcessingUnit::BufferSprites()
{
    if (!this->LCDIsOn())
    {
        return;
    }

    Address oam = ADDR_OAM_RAM_START;

    for (int i = 0; i < 40; ++i)
    {
        Byte yPos = pMemory->Read(oam++);
        Byte xPos = pMemory->Read(oam++);
        Byte tileIndex = pMemory->Read(oam++);
        Byte attr = pMemory->Read(oam++);

        Address tileAddress = ADDR_VIDEO_RAM_BLOCK_ZERO + (tileIndex * 16);

        for (int y = 0; y < 8; ++y)
        {
            for (int x = 0; x < 8; ++x)
            {
                int yAdjusted = (yPos - 16) + y;
                int xAdjusted = (xPos - 8) + x;

                if (xAdjusted >= 0 && xAdjusted <= SCREEN_WIDTH - 1 &&
                    yAdjusted >= 0 && yAdjusted <= SCREEN_HEIGHT - 1)
                {
                    Byte color = Tile::GetPixel(pMemory, tileAddress, x, y);

                    if (attr & OAM_ATTRIBUTES::OAM_DMG_PALETTE_NUM)
                    {
                        gManager.AddPixel(xAdjusted, yAdjusted, color, this->objPalette1, LCD_LAYER_OAM, true);
                    }
                    else
                    {
                        gManager.AddPixel(xAdjusted, yAdjusted, color, this->objPalette0, LCD_LAYER_OAM, true);
                    }
                }
            }
        }
    }
}

void PixelProcessingUnit::Draw()
{
    if (!this->LCDIsOn())
    {
        return;
    }

    gManager.Clear();
    gManager.Draw();
    gManager.Flush();
}

void PixelProcessingUnit::Tick(u64 cycles)
{
    this->clockCycles += cycles;

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
            if (STAT.FlagIsSet(LCD_STAT_FLAGS::HBLANK_INT_SOURCE))
            {
                this->pInterruptController->RequestInterrupt(INTERRUPT_FLAGS::INT_LCD_STAT);
            }

            this->mode = LCD_MODE::HBLANK;
            this->TestLYCMatch();
            this->STAT.ResetBit(0);
            this->STAT.ResetBit(1);
            this->clockCycles -= CLOCKS_PER_VIDEO_READ;

            if (this->STAT.FlagIsSet(LCD_STAT_FLAGS::MATCH_FLAG) && STAT.FlagIsSet(LCD_STAT_FLAGS::MATCH_INT_SOURCE))
            {
                this->pInterruptController->RequestInterrupt(INTERRUPT_FLAGS::INT_LCD_STAT);
            }
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

                if (LCDC.FlagIsSet(LCD_CTRL_FLAGS::OBJ_ON))
                {
                    this->BufferSprites();
                }

                this->iManager.HandleEvents();
                this->Draw();
                LY = 0;

#ifdef _DEBUG
                DrawTileDebug();
#endif
            }

            this->TestLYCMatch();
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

void PixelProcessingUnit::LoadColorPalette(Byte reg, Byte palette[4])
{
    for (int i = 0; i < 4; ++i)
    {
        Byte code = reg & 0b00000011;
        reg >>= 2;

        palette[i] = code;
    }
}

#ifdef _DEBUG
void PixelProcessingUnit::DrawTileDebug()
{
    if (drawDebugTile)
    {
        // Render tile map for debugging
        Address tileAddress = 0x8000;

        for (int y = 0; y < 16; ++y)
        {
            for (int x = 0; x < 24; ++x)
            {
                for (int j = 0; j < 8; ++j)
                {
                    for (int k = 0; k < 8; ++k)
                    {
                        this->tileDebugger->AddPixel(x * 8 + k, y * 8 + j, Tile::GetPixel(pMemory, tileAddress, k, j) , this->bgPalette, LCD_LAYER_BG, false);
                    }
                }

                tileAddress = tileAddress + 16;
            }
        }

        this->tileDebugger->Clear();
        this->tileDebugger->Draw();
        this->tileDebugger->Flush();
    }
}
#endif