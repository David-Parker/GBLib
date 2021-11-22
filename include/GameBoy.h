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
#include "ISerialHandler.h"
#include "MBC.h"
#include "ROM.h"

class GameBoy
{
private:
    bool romLoaded;
    std::chrono::time_point<std::chrono::high_resolution_clock> lastTimestamp;
    u64 cyclesElapsed;
    u64 framesElapsed;
    std::string romFolder;

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

public:
    GAMEBOY_API GameBoy(std::string romFolder, IGraphicsHandler* graphicsHandler, IEventHandler* eventHandler, ISerialHandler* serialHandler);
    GAMEBOY_API ~GameBoy();

    // Loads a new game rom into the GameBoy.
    void GAMEBOY_API LoadRom(std::string path);

    // Returns whether the GameBoy has requested to stop.
    bool GAMEBOY_API ShouldStop();

    // Starts the GameBoy.
    void GAMEBOY_API Start();

    // Stops the GameBoy.
    void GAMEBOY_API Stop();

    // Step through another iteration of the GameBoy emulation. Returns the number of machine cycles emulated.
    int GAMEBOY_API Step();

    // Runs the GameBoy emulation until ShouldStop() is called.
    void GAMEBOY_API Run();

    // Saves the currently loaded game if it's memory bank controller supports battery backed external RAM.
    void GAMEBOY_API SaveGame();

    // Returns the number of frames the GameBoy simulation has completed. A frame is 69905 cycles (17,476 machine cycles).
    u64 GAMEBOY_API FramesElapsed();

    // Busy waits until the end of the frame in wall clock time.
    void GAMEBOY_API SimulateFrameDelay();

    // Returns a copy of the cartridge header information, i.e. title and gb color support.
    CartridgeHeader GAMEBOY_API GetCartridgeHeader();
};