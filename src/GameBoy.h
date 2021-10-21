#pragma once
#include <chrono>
#include <string>
#include <errno.h>
#include <stdexcept>
#include "Memory.h"
#include "CartridgeHeader.h"
#include "Cpu.h"
#include "IODevices.h"
#include "IGraphicsHandler.h"
#include "IEventHandler.h"
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
    CartridgeHeader cartridgeHeader;

    void MapIODevices();
    void LoadBootRom();
    void SimulateTimeStep(int cycles);

public:
    GAMEBOY_API GameBoy(IGraphicsHandler* graphicsHandler, IEventHandler* eventHandler);
    GAMEBOY_API ~GameBoy();
    void GAMEBOY_API LoadRom(std::string path);
    void GAMEBOY_API Start();
    void GAMEBOY_API Stop();
};