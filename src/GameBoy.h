#pragma once
#include <string>
#include <errno.h>
#include <stdexcept>
#include "Memory.h"
#include "GameInfo.h"
#include "Cpu.h"
#include "IODevices.h"

class GameBoy
{
private:
    static const int ROM_SIZE = 0x8000;
    bool RomLoaded = false;

    Cpu cpu;
    Memory memory;
    IODevices devices;

    void MapIODevices();
    void LoadBootRom();
    void SimulateCycleDelay(int cycles);

public:
    GameBoy() : memory(), cpu(&memory), devices(&memory) {}
    void LoadRom(std::string path);
    void LoadTestRom();
    void Start();
    void Stop();
    GameInfo GetGameInfo();
};