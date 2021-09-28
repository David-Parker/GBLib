#pragma once
#include <string>
#include <errno.h>
#include <stdexcept>
#include "Memory.h"
#include "GameInfo.h"
#include "Cpu.h"
#include "Gpu.h"
#include "IODevices.h"

class GameBoy
{
private:
    static const int ROM_SIZE = 0x8000;
    bool RomLoaded = false;
    Cpu cpu;
    Memory memory;

    // Memory mapped I/O devices
    IODevices devices;

    void MapIODevices();
    void LoadBootRom();
    void Render();
    void SimulateCycleDelay(int cycles);

public:
    GameBoy() : memory(), cpu(&memory), devices(&memory) {}
    void LoadRom(std::string path);
    void LoadTestRom();
    void Start();
    void Stop();
    GameInfo GetGameInfo();
};