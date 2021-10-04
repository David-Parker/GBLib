#include <iostream>
#include <iomanip>
#include "Memory.h"

Memory::Memory()
    : mappedIO(), vRAM(ADDR_VIDEO_RAM_START, ADDR_VIDEO_RAM_END)
{
}

Memory::~Memory()
{
}

IMemoryMappable* Memory::IsMemoryMapped(Address address)
{
    for (auto& range : this->mappedIO)
    {
        if (address.InRange(range.low, range.high))
        {
            return range.device;
        }
    }

    return nullptr;
}

void Memory::Write(Address address, Byte value)
{
    // Checked for mapped I/O devices
    IMemoryMappable* device = this->IsMemoryMapped(address);

    if (device != nullptr)
    {
        device->Write(address, value);

        return;
    }

    /* 0x0000 - 0x3FFF (ROM) */
    /* 0x4000 - 0x7FFF (ROM SWITCHABLE) */
    /* 0x8000 - 0x9FFF (VRAM) */
    /* 0xA000 - 0xBFFF (RAM EXTERNAL CARTRIDGE) */
    /* 0xC000 - 0xDFFF (RAM) */
    /* 0xE000 - 0xFDFF (UNUSABLE) */
    /* 0xFE00 - 0xFE9F (SPRITE ATTR) */
    /* 0xFEA0 - 0xFEFF (UNUSABLE) */
    /* 0xFF00 - 0xFF70 (I/O) */
    /* 0xFF71 - 0xFF7F (UNUSABLE) */
    /* 0xFF80 - 0xFFFE (HIGH RAM) */
    /* 0xFFFF - 0xFFFF (INTERRUPT REGISTER) */

    if (address == 0xFF50 && value == 1)
    {
        // Unload boot ROM
        this->UnMapMemory(0x0);
        IMemoryMappable* gameROM = this->IsMemoryMapped(0x100);
        this->UnMapMemory(0x100);
        this->MapMemory(0x0, ROM_SIZE - 1, gameROM);
    }
    else if (
        address.InRange(0xE000, 0xFDFF) ||
        address.InRange(0xFEA0, 0xFEFF) ||
        address.InRange(0xFF4C, 0xFF7F))
    {
        throw std::exception("Attempted write to unusable address space.");
    }
    else if (address.InRange(0xFF00, 0xFF70))
    {
        throw std::exception("I/O not implemented.");
    }
    else if (address.InRange(0xFFFF, 0xFFFF))
    {
        throw std::exception("Interrupts not implemented.");
    }
    else
    {
        MEM[address] = value;
    }
}

Byte Memory::Read(Address address)
{
    // Checked for mapped I/O devices
    IMemoryMappable* device = this->IsMemoryMapped(address);

    if (device != nullptr)
    {
        return device->Read(address);
    }

    if (address.InRange(0xE000, 0xFDFF) ||
        address.InRange(0xFEA0, 0xFEFF) ||
        address.InRange(0xFF4C, 0xFF7F))
    {
        throw std::exception("Attempted read to unusable address space.");
    }

    return MEM[address];
}

void Memory::ClearMemory()
{
    for (u32 i = 0; i < Address::ADDRESSSPACE; ++i)
    {
        MEM[i] = 0;
    }

    for (Address i = ADDR_VIDEO_RAM_START; i <= ADDR_VIDEO_RAM_END; ++i)
    {
        vRAM.Write(i, 0);
    }
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
            if (i.InRange(0xE000, 0xFDFF) ||
                i.InRange(0xFEA0, 0xFEFF) ||
                i.InRange(0xFF4C, 0xFF7F))
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
    if (this->IsMemoryMapped(low) != nullptr ||
        this->IsMemoryMapped(high) != nullptr)
    {
        throw std::exception("Cannot map memory, a mapping in this range already exists.");
    }

    mappedIO.emplace_back(low, high, device);
}

void Memory::UnMapMemory(Address address)
{
    for (auto& it = this->mappedIO.begin(); it != this->mappedIO.end(); ++it)
    {
        if (address.InRange(it->low, it->high))
        {
            this->mappedIO.erase(it);
            return;
        }
    }

    throw std::exception("Cannot unmap memory, no memory mapped device found.");
}