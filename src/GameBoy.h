#pragma once
#include <string>
#include <errno.h>
#include <stdexcept>
#include "Memory.h"
#include "GameInfo.h"
#include "Cpu.h"
#include "Gpu.h"

class GameBoy
{
private:
    static const int ROM_SIZE = 0x8000;
    bool RomLoaded = false;
    Gpu gpu;
    Cpu cpu;
    Memory memory;

    void LoadBootRom();
    void Render();
    void SimulateCycleDelay(int cycles);

public:
    GameBoy() : memory(), cpu(&memory), gpu(&memory) {}
    void LoadRom(std::string path);
    void LoadTestRom();
    void Start();
    void Stop();
    GameInfo GetGameInfo();
};