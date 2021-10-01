#pragma once
#include "IMemoryMappable.h"
#include <string>

class ROM : public IMemoryMappable
{
private:
    std::string name;
    Byte* mem;
    int offset;
    bool writeDisabled;

public:
    ROM(std::string name, int offset, int size);
    ~ROM();

    void DisableWrites();
    void Write(Address address, Byte value);
    Byte Read(Address address);
};