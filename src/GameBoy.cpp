#include "GameBoy.h"
#include <thread>

using namespace std::chrono;

GameBoy::GameBoy() 
    : memory(), cpu(&memory), devices(&memory)
{
    this->MapIODevices();
    this->lastTimestamp = high_resolution_clock::now();
}

GameBoy::~GameBoy()
{
    free(this->bootROM);
    free(this->gameROM);
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
    auto diff = high_resolution_clock::now() - this->lastTimestamp;
    auto waitTo = this->lastTimestamp + std::chrono::nanoseconds(CLOCK_NS_PER_CYCLE * cycles);

    while (high_resolution_clock::now() < waitTo);

    this->lastTimestamp = high_resolution_clock::now();
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