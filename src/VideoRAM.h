#pragma once
#include "IMemoryMappable.h"
#include "GlobalDefinitions.h"

class VideoRAM :
    public IMemoryMappable
{
private:
    Byte* mem;
    bool canAccess;

public:
    VideoRAM();
    ~VideoRAM();

    void Write(Address address, Byte value);
    Byte Read(Address address);
    void EnableAccess();
    void DisableAccess();
};