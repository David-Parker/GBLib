#include "CGBRegisters.h"

CGBRegisters::CGBRegisters(Memory* pMemory) :
    pMemory(pMemory),
    VBK(),
    KEY1(),
    HDMASource(),
    HDMADest(),
    HDMA1((u8*)&HDMASource + 1),
    HDMA2((u8*)&HDMASource),
    HDMA3((u8*)&HDMADest + 1),
    HDMA4((u8*)&HDMADest),
    HDMA5(),
    RP(),
    SVBK()
{

}

void CGBRegisters::Write(Address address, Byte value)
{
    if (address == ADDR_CGB_VRAM_BANK)
    {
        VBK = value;

        if (VBK.GetBit(0) == 0)
        {
            this->pMemory->MapMemory(ADDR_VIDEO_RAM_START, ADDR_VIDEO_RAM_END, this->pMemory->vRAMBanks[0]);
        }
        else
        {
            this->pMemory->MapMemory(ADDR_VIDEO_RAM_START, ADDR_VIDEO_RAM_END, this->pMemory->vRAMBanks[1]);
        }
    }
    else if (address == ADDR_CGB_SPEED_SWITCH)
    {
        KEY1 = value;
    }
    else if (address == ADDR_CGB_HDMA1)
    {
        HDMA1 = value;
    }
    else if (address == ADDR_CGB_HDMA2)
    {
        HDMA2 = value & 0b11110000;
    }
    else if (address == ADDR_CGB_HDMA3)
    {
        HDMA3 = value & 0b00011111;
    }
    else if (address == ADDR_CGB_HDMA4)
    {
        HDMA4 = value & 0b11110000;
    }
    else if (address == ADDR_CGB_HDMA5)
    {
        // Start HDMA transfer
        HDMA5 = value;
        Address source = HDMASource;
        Address dest = ADDR_VIDEO_RAM_START + HDMADest;
        u16 length = ((HDMA5 & 0b01111111) + 1) * 0x10;

        if (AddressInRange(source, 0x7FF1, 0x9FFF) || AddressInRange(source, 0xDFF1, 0xFFFF))
        {
            throw std::runtime_error("HDMA transfer attempted on incorrect source address range.");
        }

        if (!AddressInRange(dest, 0x8000, 0x9FF0))
        {
            throw std::runtime_error("HDMA transfer attempted on incorrect destination address range.");
        }

        if (HDMA5.GetBit(7) == 0)
        {
            // General purpose DMA. Instant transfer.
            for (int i = 0; i < length; ++i)
            {
                pMemory->Write(dest + i, pMemory->Read(source + i));
            }
            
            HDMA5.SetBit(7);
        }
        else
        {
            // General purpose DMA. Instant transfer.
            for (int i = 0; i < length; ++i)
            {
                pMemory->Write(dest + i, pMemory->Read(source + i));
            }

            HDMA5.SetBit(7);
        }
    }
    else if (address == ADDR_CGB_INFRARED_PORT)
    {
        RP = value;
    }
    else if (address == ADDR_CGB_WRAM_BANK)
    {
        Byte bank = value & 0b00000111;

        if (bank == 0)
            bank = 1;

        this->pMemory->MapMemory(ADDR_WORK_RAM_BANK_1_START, ADDR_WORK_RAM_END, this->pMemory->wRAMBanks[bank]);

        SVBK = bank;
    }
}

Byte CGBRegisters::Read(Address address)
{
    if (address == ADDR_CGB_VRAM_BANK)
    {
        return VBK | 0b11111110;
    }
    else if (address == ADDR_CGB_SPEED_SWITCH)
    {
        return KEY1;
    }
    else if (address == ADDR_CGB_HDMA1)
    {
        return HDMA1;
    }
    else if (address == ADDR_CGB_HDMA2)
    {
        return HDMA2;
    }
    else if (address == ADDR_CGB_HDMA3)
    {
        return HDMA3;
    }
    else if (address == ADDR_CGB_HDMA4)
    {
        return HDMA4;
    }
    else if (address == ADDR_CGB_HDMA5)
    {
        return 0xFF;//HDMA5;
    }
    else if (address == ADDR_CGB_INFRARED_PORT)
    {
        return RP;
    }
    else if (address == ADDR_CGB_WRAM_BANK)
    {
        return SVBK;
    }

    return INVALID_READ;
}