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
#include "MBC.h"
#include "ROM.h"

class GameBoy
{
private:
    bool romLoaded = false;
    std::chrono::time_point<std::chrono::high_resolution_clock> lastTimestamp;
    u64 cyclesElapsed;
    std::string savesFolder;

    Cpu cpu;
    Memory memory;
    IODevices devices;
    ROM* gameROM;
    ROM* bootROM;
    CartridgeHeader cartridgeHeader;
    IEventHandler* eventHandler;
    IGraphicsHandler* graphicsHandler;
    MBC* mbc;

    void MapIODevices();
    void LoadBootRom();
    void SimulateTimeStep(int cycles);

public:
    GAMEBOY_API GameBoy(std::string savesFolder, IGraphicsHandler* graphicsHandler, IEventHandler* eventHandler);
    GAMEBOY_API ~GameBoy();
    void GAMEBOY_API LoadRom(std::string path);
    void GAMEBOY_API Start();
    void GAMEBOY_API Stop();
};