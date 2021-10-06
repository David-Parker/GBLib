#include "GlobalDefinitions.h"
#include "ROM.h"
#include <cstdlib>

ROM::ROM(std::string name, int offset, int size)
{
    this->name = name;
    this->offset = offset;
    this->size = size;
    this->mem = (Byte*)malloc(size);
}

ROM::~ROM()
{
    free(this->mem);
}

void ROM::LoadFromFile(std::string path)
{
    FILE *filepoint;
    errno_t err;

    if ((err = fopen_s(&filepoint, path.c_str(), "rb")) != 0)
    {
        throw std::exception("Could not open ROM file.");
    }
    else
    {
        size_t bytesRead = fread(this->mem, sizeof(Byte), this->size, filepoint);

        if (bytesRead == 0)
        {
            throw std::exception("Failed to read ROM file.");
        }

        fclose(filepoint);
    }
}

void ROM::Write(Address address, Byte value)
{
    // No op. Some games incorrectly try to write to ROM, therefore we cannot throw an exception here, and will instead do nothing.
}

Byte ROM::Read(Address address)
{
    return this->mem[address - offset];
}