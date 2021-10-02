#include "GameBoy.h"
#include <chrono>
#include <thread>

using namespace std::chrono;

GameBoy::GameBoy() 
    : memory(), cpu(&memory), devices(&memory)
{
    this->MapIODevices();
}

// Loads the 256-byte boot ROM into addresses 0x00 to 0xFF
void GameBoy::LoadBootRom()
{
    this->bootROM = new ROM("boot", 0, 256);
    this->bootROM->LoadFromFile("rom/boot.bin");
    this->memory.MapMemory(0x00, 0xFF, this->bootROM);
}

// Loads the ROM file into memory
void GameBoy::LoadRom(std::string path)
{
    // Clear memory
    memory.ClearMemory();
    LoadBootRom();

    this->gameROM = new ROM("game", 0, ROM_SIZE);
    this->gameROM->LoadFromFile(path);
    this->memory.MapMemory(0x100, ROM_SIZE - 1, this->gameROM); // ROM 0x00 to 0xFF is mapped after boot sequence is completed.

    RomLoaded = true;
}

void GameBoy::MapIODevices()
{
    this->memory.MapMemory(ADDR_IF, ADDR_IF, &devices.interruptController);
    this->memory.MapMemory(ADDR_IE, ADDR_IE, &devices.interruptController);
    this->memory.MapMemory(ADDR_SOUND_START, ADDR_SOUND_END, &devices.soundController);
    this->memory.MapMemory(ADDR_PPU_START, ADDR_PPU_END, &devices.ppu);
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

            this->devices.ppu.Tick(cycleCount);

            //this->SimulateCycleDelay(cycles * CLOCK_CYCLES_PER_MACHINE_CYCLE);
            //std::this_thread::sleep_for(10ms);
        }
    }
    catch (std::exception& ex)
    {
        std::cout << "Exception encountered: " << ex.what() << std::endl;
        this->cpu.StopCPU();
    }
}

void GameBoy::Stop()
{

}

void GameBoy::SimulateCycleDelay(int cycles)
{
    auto t1 = high_resolution_clock::now();

    while (1)
    {
        auto t2 = high_resolution_clock::now();

        if (duration_cast<nanoseconds>(t2 - t1).count() >= cycles * CLOCK_NS_PER_CYCLE)
        {
            return;
        }
    }
}

GameInfo GameBoy::GetGameInfo()
{
    if (!RomLoaded)
    {
        throw std::exception("Can not get GameInfo. No ROM is loaded.");
    }

    GameInfo info;
    info.Read(&memory);

    return info;
}