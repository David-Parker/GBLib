#include "GlobalDefinitions.h"
#include "ROM.h"
#include <cstdlib>

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
    FILE *file;
    errno_t err;

    if ((err = fopen_s(&file, path.c_str(), "rb")) != 0)
    {
        throw std::exception("Could not open ROM file.");
    }
    else
    {
        fseek(file, (long)start, SEEK_SET);

        size_t bytesRead = fread(this->mem, sizeof(Byte), bytes, file);

        if (bytesRead != bytes)
        {
            throw std::exception("Failed to read ROM file.");
        }

        fclose(file);
    }
}

void ROM::Write(Address address, Byte value)
{
    // No op. Some games incorrectly try to write to ROM, therefore we cannot throw an exception here, and will instead do nothing.
}

Byte ROM::Read(Address address)
{
    return this->mem[address - this->start];
}