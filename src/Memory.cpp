#include <iostream>
#include <iomanip>
#include "Memory.h"

Memory::Memory()
    : mappedIO()
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

        // Also write value to "RAM" for debugging purposes.
        RAM[address] = value;
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
    /* 0xFF00 - 0xFF4B (I/O) */
    /* 0xFF4C - 0xFF7F (UNUSABLE) */
    /* 0xFF80 - 0xFFFE (HIGH RAM) */
    /* 0xFFFF - 0xFFFF (INTERRUPT REGISTER) */

    if (address.InRange(0xE000, 0xFDFF) ||
        address.InRange(0xFEA0, 0xFEFF) ||
        address.InRange(0xFF4C, 0xFF7F))
    {
        throw std::exception("Attempted write to unusable address space.");
    }

    if (address.InRange(0xFF00, 0xFF4B))
    {
        throw std::exception("I/O not implemented.");
    }

    if (address.InRange(0xFFFF, 0xFFFF))
    {
        throw std::exception("Interrupts not implemented.");
    }


    RAM[address] = value;
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

    return RAM[address];
}

void Memory::ClearMemory()
{
    for (u32 i = 0; i < Address::ADDRESSSPACE; ++i)
    {
        RAM[i] = 0;
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
        for (u32 i = start; i <= end; i++)
        {
            fprintf(file, "[0x%04X]: 0x%04X\n", i, RAM[i]);
        }

        fclose(file);
    }
}

void Memory::MapMemory(Address low, Address high, IMemoryMappable* device)
{
    mappedIO.emplace_back(low, high, device);
}