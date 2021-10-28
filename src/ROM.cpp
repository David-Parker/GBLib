#include "GlobalDefinitions.h"
#include "ROM.h"
#include<iostream>
#include<fstream>

ROM::ROM(Address start, Address end)
{
    this->start = start;
    this->end = end;
    this->mem = (Byte*)malloc((end - start) + 1);
}

ROM::~ROM()
{
    free(this->mem);
}

void ROM::LoadFromFile(std::string path, size_t start, size_t bytes)
{
    std::ifstream file(path, std::ios::binary);

    // No save file exists
    if (!file)
    {
        throw std::runtime_error("Could not open ROM file.");
    }

    file.seekg(start);

    file.read(reinterpret_cast<char*>(this->mem), bytes);
}

void ROM::Write(Address address, Byte value)
{
    // No op. Some games incorrectly try to write to ROM, therefore we cannot throw an exception here, and will instead do nothing.
}

Byte ROM::Read(Address address)
{
    return this->mem[address - this->start];
}