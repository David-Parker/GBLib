#pragma once
#include "IMemoryMappable.h"
#include <string>

class ROM : public IMemoryMappable
{
private:
    Byte* mem;
    Address start;
    Address end;

public:
    ROM(Address start, Address end);
    ~ROM();

    void LoadFromFile(std::string path, size_t start, size_t bytes);
    void Write(Address address, Byte value);
    Byte Read(Address address);
};