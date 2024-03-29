#include "PixelProcessingUnit.h"
#include "IGraphicsHandler.h"
#include <thread>
#include <algorithm>

PixelProcessingUnit::PixelProcessingUnit(Memory* pMemory, InterruptController* interruptController, IGraphicsHandler* graphicsHandler, IEventHandler* eventHandler)
    :   pMemory(pMemory),
        pInterruptController(interruptController),
        backgroundMap(pMemory), 
        clockCycles(0),
        spriteCount(0),
        mode(LCD_MODE::OAM_SEARCH),
        lcdInitialized(false),
        gManager(graphicsHandler, SCREEN_WIDTH, SCREEN_HEIGHT, LCD_NUM_LAYERS),
        eventHandler(eventHandler),
        LCDC(&mem[ADDR_PPU_REG_CONTROL - ADDR_PPU_START]),
        STAT(&mem[ADDR_PPU_REG_STATUS - ADDR_PPU_START]),
        SCY(&mem[ADDR_PPU_REG_SCROLL_Y - ADDR_PPU_START]),
        SCX(&mem[ADDR_PPU_REG_SCROLL_X - ADDR_PPU_START]),
        LY(&mem[ADDR_PPU_REG_LY - ADDR_PPU_START]),
        LYC(&mem[ADDR_PPU_REG_LY_COMPARE - ADDR_PPU_START]),
        DMA(&mem[ADDR_PPU_REG_DMA_TRANSFER - ADDR_PPU_START]),
        BGP(&mem[ADDR_PPU_REG_BG_PALETTE_DATA - ADDR_PPU_START]),
        OBP0(&mem[ADDR_PPU_REG_OBJ_PALETTE_0_DATA - ADDR_PPU_START]),
        OBP1(&mem[ADDR_PPU_REG_OBJ_PALETTE_1_DATA - ADDR_PPU_START]),
        WY(&mem[ADDR_PPU_REG_OBJ_WINDOW_Y_POS - ADDR_PPU_START]),
        WX(&mem[ADDR_PPU_REG_OBJ_WINDOW_X_POS_MIN_7 - ADDR_PPU_START])
{
#ifdef _DEBUG
    if (this->drawDebugTile)
    {
        this->tileDebugger = new GraphicsManager(graphicsHandler, 192, 128, 1);
        this->tileDebugger->Init();
    }
#endif

    this->lastFrameTime = std::chrono::high_resolution_clock::now();
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

Address PixelProcessingUnit::GetTileData()
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

Address PixelProcessingUnit::GetWindowTileMap()
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
    else if (address == ADDR_PPU_REG_LY)
    {
        return;
    }
    else if (address == ADDR_PPU_REG_LY_COMPARE)
    {
        LYC = value;
        this->TestLYCMatch();
    }
    else if (address == ADDR_PPU_REG_DMA_TRANSFER)
    {
        Address source = value * 0x100;
        Address dest = ADDR_OAM_RAM_START;

        // DMA is instant. On the real GameBoy, this operation takes several clock cycles.
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
    else if (address == ADDR_PPU_REG_SCROLL_X)
    {
        SCX = value;
    }
    else if (address == ADDR_CGB_OBJ_PRIORITY)
    {
        OPRI = value;
    }
    else
    {
        mem[address - ADDR_PPU_START] = value;
    }
}

Byte PixelProcessingUnit::Read(Address address)
{
    if (address == ADDR_CGB_OBJ_PRIORITY)
    {
        return OPRI;
    }

    return mem[address - ADDR_PPU_START];
}

void PixelProcessingUnit::TurnOnLCD()
{
    if (!this->lcdInitialized)
    {
        this->gManager.Init();
        this->lcdInitialized = true;
        this->ResetLCD();
    }
}

void PixelProcessingUnit::TurnOffLCD()
{
    if (this->lcdInitialized)
    {
        this->gManager.Clear();
    }
}

void PixelProcessingUnit::ResetLCD()
{
    this->clockCycles = 0;
    this->LY = 0;
    this->WLY = 0;
    this->TestLYCMatch();
    this->EnterOamSearch();
}

bool PixelProcessingUnit::LCDIsOn()
{
    return LCDC.FlagIsSet(LCD_CTRL_FLAGS::LCD_ON);
}

void PixelProcessingUnit::BufferScanLine()
{
    if (!this->LCDIsOn() || !this->canDraw)
    {
        return;
    }

    if (LCDC.FlagIsSet(LCD_CTRL_FLAGS::BG_WIN_DISPLAY_ON))
    {
        RenderBG();

        if (LCDC.FlagIsSet(LCD_CTRL_FLAGS::WINDOWING_ON))
        {
            RenderWindow();
        }
    }

    if (LCDC.FlagIsSet(LCD_CTRL_FLAGS::OBJ_ON))
    {
        RenderSprites();
    }
}

void PixelProcessingUnit::RenderBG()
{
    for (int i = 0; i < SCREEN_WIDTH; i++)
    {
        Byte color = this->backgroundMap.GetPixel(GetTileData(), GetBGTileMap(), (i + SCX) % 256, (LY + SCY) % 256);
        this->bgWinColor[LY][i] = color;
        gManager.AddPixel(i, LY, color, this->bgPalette, LCD_LAYER_BG);
    }
}

void PixelProcessingUnit::RenderWindow()
{
    if (WLY >= SCREEN_HEIGHT)
        return;

    int xAdjusted = WX - 7;

    if (xAdjusted >= SCREEN_WIDTH)
        return;

    if (WY >= SCREEN_HEIGHT || WY > LY)
        return;

    for (int i = xAdjusted; i < SCREEN_WIDTH; i++)
    {
        if (i < 0)
            continue;

        Byte color = this->backgroundMap.GetPixel(GetTileData(), GetWindowTileMap(), i - xAdjusted, WLY);
        this->bgWinColor[LY][i] = color;
        gManager.AddPixel(i, LY, color, this->bgPalette, LCD_LAYER_WIN);
    }

    ++WLY;
}

void PixelProcessingUnit::RenderSprites()
{
    for (int i = 0; i < this->spriteCount; ++i)
    {
        Sprite* sprite = &this->sprites[i];

        Address tileAddress = ADDR_VIDEO_RAM_BLOCK_ZERO + (sprite->tileIndex * 16);

        for (int x = 0; x < 8; ++x)
        {
            int xAdjusted = (sprite->xPos - 8) + x;

            // Sprite is visible horizontally
            if (xAdjusted >= 0 && xAdjusted <= SCREEN_WIDTH - 1)
            {
                int y = LY - (sprite->yPos - 16);

                u8 layer = LCD_LAYER_OBJ_TOP;
                int xFlip = sprite->attr & OAM_ATTRIBUTES::OAM_X_FLIP ? 7 - x : x;
                int yFlip = sprite->attr & OAM_ATTRIBUTES::OAM_Y_FLIP ? (sprite->height - 1) - y : y;

                Byte color = Tile::GetPixel(pMemory, tileAddress, xFlip, yFlip);
                Byte* palette = sprite->attr & OAM_ATTRIBUTES::OAM_DMG_PALETTE_NUM ? this->objPalette1 : this->objPalette0;

                if (color == 0)
                {
                    continue;
                }

                // BG and WIN colors 1-3 over sprite
                if (sprite->attr & OAM_ATTRIBUTES::OAM_BG_WIN_OVER_OBJ)
                {
                    Byte color = this->bgWinColor[LY][xAdjusted];

                    if (color != 0)
                        layer = LCD_LAYER_OBJ_BOTTOM;
                }

                gManager.AddPixel(xAdjusted, LY, color, palette, layer);
            }
        }
    }
}

bool SpriteCompareOAM(Sprite const& lhs, Sprite const& rhs)
{
    return lhs.index > rhs.index;
}

bool SpriteCompareXCoord(Sprite const& lhs, Sprite const& rhs) 
{
    // If sprites share the same X-coordinate, the first in OAM takes priority.
    if (lhs.xPos == rhs.xPos)
        return SpriteCompareOAM(lhs, rhs);

    return lhs.xPos > rhs.xPos;
}

void PixelProcessingUnit::SearchSprites()
{
    this->spriteCount = 0;

    Address oam = ADDR_OAM_RAM_START;

    for (int i = 0; i < SPRITE_NUM_OBJS; ++i)
    {
        if (this->spriteCount == SPRITE_OBJ_LIMIT)
        {
            continue;
        }

        Byte yPos = pMemory->Read(oam++);
        Byte xPos = pMemory->Read(oam++);
        Byte tileIndex = pMemory->Read(oam++);
        Byte attr = pMemory->Read(oam++);

        // 8x8 and 8x16 sprites are "fully visible" at yPos 16.
        // Adjust the yPos by 16, so the top line is at pos 0.
        Byte yTop = yPos - 16;
        Byte yBottom = LCDC.FlagIsSet(LCD_CTRL_FLAGS::OBJ_SIZE) ? yTop + 16 : yTop + 8;

        // Sprite is in this scanline
        if (LY >= yTop && LY < yBottom)
        {
            this->sprites[this->spriteCount].yPos = yPos;
            this->sprites[this->spriteCount].xPos = xPos;
            this->sprites[this->spriteCount].tileIndex = tileIndex;
            this->sprites[this->spriteCount].attr = attr;
            this->sprites[this->spriteCount].height = LCDC.FlagIsSet(LCD_CTRL_FLAGS::OBJ_SIZE) ? 16 : 8;
            this->sprites[this->spriteCount].index = this->spriteCount;

            // 8x16 sprite's tile index bit 0 is always set to 0
            if (this->sprites[this->spriteCount].height == 16)
                this->sprites[this->spriteCount].tileIndex &= 0b11111110;

            this->spriteCount++;
        }
    }

    // Sprites are drawn either in the order of the highest X coordinate first
    // Or in the order they appear in OAM RAM (for CGB only)
    if (this->emuType == EMUType::CGB && !OPRI.FlagIsSet(OBJ_PRIO_MODE))
    {
        std::sort(this->sprites, this->sprites + this->spriteCount, &SpriteCompareOAM);
    }
    else
    {
        std::sort(this->sprites, this->sprites + this->spriteCount, &SpriteCompareXCoord);
    }
}

void PixelProcessingUnit::Draw()
{
    if (!this->LCDIsOn() || !this->canDraw)
    {
        return;
    }

    memset(bgWinColor, 0, SCREEN_HEIGHT * SCREEN_WIDTH);
    gManager.Draw();

    this->canDraw = false;
}

void PixelProcessingUnit::Tick(u64 cycles)
{
    this->clockCycles += cycles;

    switch (this->mode)
    {
    case LCD_MODE::OAM_SEARCH:
        if (this->clockCycles >= CLOCKS_PER_OAM_SEARCH)
        {
            this->ExitOamSearch();
            this->EnterVideoRead();
            
            this->clockCycles -= CLOCKS_PER_OAM_SEARCH;
        }
        break;
    case LCD_MODE::VIDEO_READ:
        if (this->clockCycles >= CLOCKS_PER_VIDEO_READ)
        {
            this->ExitVideoRead();
            this->EnterHBlank();

            this->clockCycles -= CLOCKS_PER_VIDEO_READ;
        }
        break;
    case LCD_MODE::HBLANK:
        if (this->clockCycles >= CLOCKS_PER_HBLANK)
        {
            this->ExitHBlank();

            if (LY < SCREEN_HEIGHT)
            {
                this->EnterOamSearch();
            }
            else
            {
                this->EnterVBlank();
            }

            this->clockCycles -= CLOCKS_PER_HBLANK;
        }
        break;
    case LCD_MODE::VBLANK:
        if (this->clockCycles >= CLOCKS_PER_VBLANK)
        {
            ++LY;
            this->TestLYCMatch();

            if (LY == SCREEN_HEIGHT + 10)
            {
                this->ExitVBlank();
                this->EnterOamSearch();
#ifdef _DEBUG
                DrawTileDebug();
#endif
            }

            this->clockCycles -= CLOCKS_PER_VBLANK;
        }
        break;
    default:
        throw std::runtime_error("Unknown LCD Mode.");
    }
}

void PixelProcessingUnit::EnterOamSearch()
{
    if (STAT.FlagIsSet(LCD_STAT_FLAGS::OAM_INT_SOURCE))
    {
        this->pInterruptController->RequestInterrupt(INTERRUPT_FLAGS::INT_LCD_STAT);
    }

    this->mode = LCD_MODE::OAM_SEARCH;
    this->STAT.ResetBit(0);
    this->STAT.SetBit(1);
}

void PixelProcessingUnit::ExitOamSearch()
{
    this->SearchSprites();
}

void PixelProcessingUnit::EnterVideoRead()
{
    this->mode = LCD_MODE::VIDEO_READ;
    this->STAT.SetBit(0);
    this->STAT.SetBit(1);
}

void PixelProcessingUnit::ExitVideoRead()
{
    this->BufferScanLine();
}

void PixelProcessingUnit::EnterHBlank()
{
    if (STAT.FlagIsSet(LCD_STAT_FLAGS::HBLANK_INT_SOURCE))
    {
        this->pInterruptController->RequestInterrupt(INTERRUPT_FLAGS::INT_LCD_STAT);
    }

    this->mode = LCD_MODE::HBLANK;
    this->STAT.ResetBit(0);
    this->STAT.ResetBit(1);
}

void PixelProcessingUnit::ExitHBlank()
{
    ++LY;
    this->TestLYCMatch();
}

void PixelProcessingUnit::EnterVBlank()
{
    if (STAT.FlagIsSet(LCD_STAT_FLAGS::VBLANK_INT_SOURCE))
    {
        this->pInterruptController->RequestInterrupt(INTERRUPT_FLAGS::INT_LCD_STAT);
    }

    this->mode = LCD_MODE::VBLANK;
    this->STAT.SetBit(0);
    this->STAT.ResetBit(1);
    this->pInterruptController->RequestInterrupt(INTERRUPT_FLAGS::INT_VBLANK);
}

void PixelProcessingUnit::ExitVBlank()
{
    this->Draw();
    LY = 0;
    WLY = 0;
    this->TestLYCMatch();

    // Check if another 16.7ms of real time has elapsed. Enable drawing for the next frame.
    auto delta = std::chrono::high_resolution_clock::now() - this->lastFrameTime;

    if (delta >= std::chrono::nanoseconds(CLOCK_NS_PER_FRAME))
    {
        this->canDraw = true;
        this->lastFrameTime += delta;
    }
}

void PixelProcessingUnit::TestLYCMatch()
{
    if (LY == LYC)
    {
        this->STAT.SetBit(LCD_STAT_FLAGS::MATCH_FLAG);

        if (STAT.FlagIsSet(LCD_STAT_FLAGS::MATCH_INT_SOURCE))
        {
            this->pInterruptController->RequestInterrupt(INTERRUPT_FLAGS::INT_LCD_STAT);
        }
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

void PixelProcessingUnit::SetEMUType(EMUType emuType)
{
    this->emuType = emuType;
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
                        this->tileDebugger->AddPixel(x * 8 + k, y * 8 + j, Tile::GetPixel(pMemory, tileAddress, k, j) , this->bgPalette, LCD_LAYER_BG);
                    }
                }

                tileAddress = tileAddress + 16;
            }
        }

        this->tileDebugger->Draw();
    }
}
#endif