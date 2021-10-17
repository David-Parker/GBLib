#include "GameBoy.h"
#include <thread>

using namespace std::chrono;

GameBoy::GameBoy() 
    : memory(), devices(&memory), cpu(&memory, &devices.interruptController), inputManager(&memory, &devices.interruptController, &devices.joypadController)
{
    this->MapIODevices();
    this->lastTimestamp = high_resolution_clock::now();
}

GameBoy::~GameBoy()
{
    delete this->bootROM;
    delete this->gameROM;
}

// Loads the 256-byte boot ROM into addresses 0x00 to 0xFF
void GameBoy::LoadBootRom()
{
    this->bootROM = new ROM("boot", 0, 256);
    this->bootROM->LoadFromFile("rom/boot.bin");
    this->memory.MapMemory(ADDR_BOOT_ROM_START, ADDR_BOOT_ROM_END, this->bootROM);
}

// Loads the ROM file into memory
void GameBoy::LoadRom(std::string path)
{
#ifndef _DEBUG
    LoadBootRom();
#endif

    this->gameROM = new ROM("game", 0, ROM_SIZE);
    this->gameROM->LoadFromFile(path);

#ifndef _DEBUG
    this->memory.MapMemory(ADDR_BOOT_ROM_END + 1, ROM_SIZE - 1, this->gameROM); // Game ROM 0x00 to 0xFF is mapped after boot sequence is completed.
#else
    this->memory.MapMemory(0, ROM_SIZE - 1, this->gameROM); // Game ROM 0x00 to 0xFF is mapped after boot sequence is completed.
#endif

    this->romLoaded = true;
}

void GameBoy::MapIODevices()
{
    this->memory.MapMemory(ADDR_INTERRUPT_FLAG, ADDR_INTERRUPT_FLAG, &devices.interruptController);
    this->memory.MapMemory(ADDR_INTERRUPT_ENABLE, ADDR_INTERRUPT_ENABLE, &devices.interruptController);
    this->memory.MapMemory(ADDR_SOUND_START, ADDR_SOUND_END, &devices.soundController);
    this->memory.MapMemory(ADDR_PPU_START, ADDR_PPU_END, &devices.ppu);
    this->memory.MapMemory(ADDR_JOYPAD_INPUT, ADDR_JOYPAD_INPUT, &devices.joypadController);
    this->memory.MapMemory(ADDR_SERIAL_START, ADDR_SERIAL_END, &devices.serialController);
    this->memory.MapMemory(ADDR_TIMER_START, ADDR_TIMER_END, &devices.timerController);
}

void GameBoy::Start()
{
    try
    {
        u64 cycleCount = 0;

        this->cpu.StartCPU();

        while (this->cpu.IsRunning())
        {
            int cycles = this->cpu.Tick();

            cycleCount += cycles;

            this->devices.ppu.Tick(cycles);
            this->devices.timerController.Tick(cycles);
            this->inputManager.HandleEvents();
            this->SimulateTimeStep(cycles);
        }
    }
    catch (std::exception& ex)
    {
        std::cout << "Exception encountered: " << ex.what() << std::endl;
        this->Stop();
    }
}

void GameBoy::Stop()
{
    this->cpu.StopCPU();
}

void GameBoy::SimulateTimeStep(int cycles)
{
#ifdef _DEBUG
    return;
#endif

    auto waitTo = this->lastTimestamp + std::chrono::nanoseconds(CLOCK_NS_PER_CYCLE /** CLOCK_CYCLES_PER_MACHINE_CYCLE*/ * cycles);

    while (high_resolution_clock::now() < waitTo);

    this->lastTimestamp = waitTo;
}

GameInfo GameBoy::GetGameInfo()
{
    if (!this->romLoaded)
    {
        throw std::exception("Can not get GameInfo. No ROM is loaded.");
    }

    GameInfo info;
    info.Read(&memory);

    return info;
}