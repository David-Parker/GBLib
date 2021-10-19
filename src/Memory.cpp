#include <iostream>
#include <iomanip>
#include "Memory.h"

Memory::Memory()
    :   vRAM(ADDR_VIDEO_RAM_START, ADDR_VIDEO_RAM_END),
        eRAM(ADDR_EXTERNAL_RAM_START, ADDR_EXTERNAL_RAM_END),
        gRAM(ADDR_GENERAL_RAM_START, ADDR_GENERAL_RAM_END),
        oRAM(ADDR_OAM_RAM_START, ADDR_OAM_RAM_END),
        hRAM(ADDR_HIGH_RAM_START, ADDR_HIGH_RAM_END)
{
    this->MapMemory(0x00, ADDRESSSPACE - 1, &this->unMapped);
    this->MapMemory(ADDR_VIDEO_RAM_START, ADDR_VIDEO_RAM_END, &this->vRAM);
    this->MapMemory(ADDR_EXTERNAL_RAM_START, ADDR_EXTERNAL_RAM_END, &this->eRAM);
    this->MapMemory(ADDR_GENERAL_RAM_START, ADDR_GENERAL_RAM_END, &this->gRAM);
    this->MapMemory(ADDR_OAM_RAM_START, ADDR_OAM_RAM_END, &this->oRAM);
    this->MapMemory(ADDR_HIGH_RAM_START, ADDR_HIGH_RAM_END, &this->hRAM);
}

Memory::~Memory()
{
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
    if (address == 0xFF50)
    {
        if (value == 1)
        {
            // Unload boot ROM
            IMemoryMappable* gameROM = this->addressSpace[0x100];
            this->MapMemory(0x0, ROM_SIZE - 1, gameROM);
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
    FILE *file;
    errno_t err;

    if ((err = fopen_s(&file, "dump.txt", "wb")) != 0)
    {
        throw std::exception("Could not create dump file.");
    }
    else
    {
        for (Address i = start; i <= end; i++)
        {
            if (this->addressSpace[i] == &this->unMapped)
            {
                fprintf(file, "[0x%04X]: 0x0000\n", (int)i);
            }
            else
            {
                fprintf(file, "[0x%04X]: 0x%04X\n", (int)i, Read(i));
            }
        }

        fclose(file);
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