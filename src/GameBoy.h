#pragma once
#include <string>
#include <errno.h>
#include <stdexcept>
#include "Memory.h"
#include "GameInfo.h"
#include "Cpu.h"
#include "IODevices.h"
#include "ROM.h"
#include "VideoRAM.h"

class GameBoy
{
private:
    bool RomLoaded = false;

    Cpu cpu;
    Memory memory;
    IODevices devices;
    ROM* gameROM;
    ROM* bootROM;

    void MapIODevices();
    void LoadBootRom();
    void SimulateCycleDelay(int cycles);

public:
    GameBoy();
    void LoadRom(std::string path);
    void Start();
    void Stop();
    GameInfo GetGameInfo();
};