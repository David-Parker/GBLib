#include "VideoRAM.h"
#include "Address.h"

VideoRAM::VideoRAM()
{
    this->mem = (Byte*)malloc((ADDR_VIDEO_RAM_END - ADDR_VIDEO_RAM_START) + 1);
    this->canAccess = true;
}

VideoRAM::~VideoRAM()
{
    free(this->mem);
}

void VideoRAM::Write(Address address, Byte value)
{
    if (!this->canAccess)
    {
        return;
    }

    this->mem[address - ADDR_VIDEO_RAM_START] = value;
}

Byte VideoRAM::Read(Address address)
{
    if (!this->canAccess)
    {
        return 0xFF;
    }

    return this->mem[address - ADDR_VIDEO_RAM_START];
}

void VideoRAM::EnableAccess()
{
    this->canAccess = true;
}

void VideoRAM::DisableAccess()
{
    this->canAccess = false;
}