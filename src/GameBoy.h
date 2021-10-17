#pragma once
#include <chrono>
#include <string>
#include <errno.h>
#include <stdexcept>
#include "Memory.h"
#include "GameInfo.h"
#include "Cpu.h"
#include "IODevices.h"
#include "InputManager.h"
#include "ROM.h"

class GameBoy
{
private:
    bool romLoaded = false;
    std::chrono::steady_clock::time_point lastTimestamp;
    u64 cyclesElapsed;

    Cpu cpu;
    Memory memory;
    IODevices devices;
    ROM* gameROM;
    ROM* bootROM;

    void MapIODevices();
    void LoadBootRom();
    void SimulateTimeStep(int cycles);

public:
    GameBoy();
    ~GameBoy();
    void LoadRom(std::string path);
    void Start();
    void Stop();
    GameInfo GetGameInfo();
};