#include <iostream>
#include <iomanip>
#include "Memory.h"

Memory::Memory()
    :   eRAM(ADDR_EXTERNAL_RAM_START, ADDR_EXTERNAL_RAM_END),
        oRAM(ADDR_OAM_RAM_START, ADDR_OAM_RAM_END),
        hRAM(ADDR_HIGH_RAM_START, ADDR_HIGH_RAM_END),
        vRAMBanks(),
        wRAMBanks()
{
    this->vRAMBanks.push_back(new RAM(ADDR_VIDEO_RAM_START, ADDR_VIDEO_RAM_END));
    this->wRAMBanks.push_back(new RAM(ADDR_WORK_RAM_START, ADDR_WORK_RAM_BANK_0_END));
    this->wRAMBanks.push_back(new RAM(ADDR_WORK_RAM_BANK_1_START, ADDR_WORK_RAM_END));

    this->MapMemory(0x00, ADDRESSSPACE - 1, &this->unMapped);
    this->MapMemory(ADDR_EXTERNAL_RAM_START, ADDR_EXTERNAL_RAM_END, &this->eRAM);
    this->MapMemory(ADDR_VIDEO_RAM_START, ADDR_VIDEO_RAM_END, this->vRAMBanks[0]);
    this->MapMemory(ADDR_WORK_RAM_START, ADDR_WORK_RAM_BANK_0_END, this->wRAMBanks[0]);
    this->MapMemory(ADDR_WORK_RAM_BANK_1_START, ADDR_WORK_RAM_END, this->wRAMBanks[1]);
    this->MapMemory(ADDR_OAM_RAM_START, ADDR_OAM_RAM_END, &this->oRAM);
    this->MapMemory(ADDR_HIGH_RAM_START, ADDR_HIGH_RAM_END, &this->hRAM);
}

Memory::~Memory()
{
    for (auto ram : this->vRAMBanks)
    {
        delete ram;
    }

    for (auto ram : this->wRAMBanks)
    {
        delete ram;
    }
}

/* 0x0000 - 0x3FFF (ROM) */
/* 0x4000 - 0x7FFF (ROM SWITCHABLE) */
/* 0x8000 - 0x9FFF (VRAM) */
/* 0xA000 - 0xBFFF (RAM EXTERNAL CARTRIDGE) */
/* 0xC000 - 0xDFFF (RAM) */
/* 0xE000 - 0xFDFF (UNUSABLE) */
/* 0xFE00 - 0xFE9F (OAM RAM) */
/* 0xFEA0 - 0xFEFF (UNUSABLE) */
/* 0xFF00 - 0xFF7F (I/O) */
/* 0xFF80 - 0xFFFE (HIGH RAM) */
/* 0xFFFF - 0xFFFF (INTERRUPT REGISTER) */
void Memory::Write(Address address, Byte value)
{
    if (address == ADDR_SPECIAL_REG_BOOT_UNLOAD)
    {
        if (value == 0x01 || value == 0xFF)
        {
            // Unload DMG boot ROM
            IMemoryMappable* gameROM = this->addressSpace[0x100];
            this->MapMemory(ADDR_BOOT_ROM_DMG_START, ADDR_BOOT_ROM_DMG_END, gameROM);
        }
        else if (value == 0x11)
        {
            // Unload CGB boot ROM
            IMemoryMappable* gameROM = this->addressSpace[0x100];
            this->MapMemory(ADDR_BOOT_ROM_CGB_START, ADDR_BOOT_ROM_CGB_END, gameROM);
        }
    }
    else
    {
        this->addressSpace[address]->Write(address, value);
    }
}

void Memory::ClearMemory()
{
    // TODO
}

void Memory::Dump(Address start, Address end)
{
    std::ofstream file("dump.debug", std::fstream::out | std::ios::binary | std::ios::trunc);

    if (!file)
    {
        throw std::runtime_error("Could not create dump file.");
    }
    else
    {
        char buf[64];

        for (Address i = start; i <= end; i++)
        {
            if (this->addressSpace[i] == &this->unMapped)
            {
                snprintf(buf, 64, "[0x%04X]: 0x0000\n", (int)i);
            }
            else
            {
                snprintf(buf, 64, "[0x%04X]: 0x%04X\n", (int)i, Read(i));
            }

            file.write(buf, 64);
        }
    }
}

void Memory::MapMemory(Address low, Address high, IMemoryMappable* device)
{
    for (int i = low; i <= high; ++i)
    {
        this->addressSpace[i] = device;
    }
}

void Memory::UnMapMemory(Address low, Address high)
{
    for (int i = low; i <= high; ++i)
    {
        this->addressSpace[i] = &this->unMapped;
    }
}

void Memory::CreateCGBBanks()
{
    this->vRAMBanks.push_back(new RAM(ADDR_VIDEO_RAM_START, ADDR_VIDEO_RAM_END));

    for (int i = 0; i < 7; ++i)
    {
        this->wRAMBanks.push_back(new RAM(ADDR_WORK_RAM_BANK_1_START, ADDR_WORK_RAM_END));
    }
}