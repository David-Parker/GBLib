#include "GameBoy.h"
#include <chrono>
#include <thread>

using namespace std::chrono;

// Loads the 256-byte boot ROM into addresses 0x00 to 0xFF
void GameBoy::LoadBootRom()
{
    unsigned currentAddress = 0x0;
    unsigned char buffer[256];
    FILE *filepoint;
    errno_t err;
    std::string path = "rom/boot.bin";

    if ((err = fopen_s(&filepoint, path.c_str(), "rb")) != 0)
    {
        throw std::exception("Could not open boot ROM file.");
    }
    else
    {
        size_t bytes = fread(buffer, sizeof(unsigned char), 256, filepoint);

        for (u32 i = 0; i < bytes; i++)
        {
            memory.Write(i, buffer[i]);
        }

        fclose(filepoint);
    }
}

// Loads the ROM file into memory
void GameBoy::LoadRom(std::string path)
{
    // Clear memory
    memory.ClearMemory();

    unsigned currentAddress = 0x000; // Game Code starts at address 100, load scrolling graphic at 104
    unsigned char buffer[ROM_SIZE];
    FILE *filepoint;
    errno_t err;

    if ((err = fopen_s(&filepoint, path.c_str(), "rb")) != 0)
    {
        throw std::exception("Could not open ROM file.");
    }
    else
    {
        size_t bytes = fread(buffer, sizeof(unsigned char), ROM_SIZE, filepoint);

        for (u32 i = currentAddress; i < bytes; i++)
        {
            memory.Write(i, buffer[i]);
        }

        fclose(filepoint);
    }

    LoadBootRom();

    RomLoaded = true;

    memory.Dump(0x0000, 0x3FFF);
}

void GameBoy::LoadTestRom()
{
    // Clear memory
    memory.ClearMemory();

    LoadBootRom();

    unsigned currentAddress = 0x00; // Game Code starts at address 100, load scrolling graphic at 104
    unsigned char buffer[ROM_SIZE];
    FILE *filepoint;
    errno_t err;

    if ((err = fopen_s(&filepoint, "rom/tests/cpu_instrs.gb", "rb")) != 0)
    {
        throw std::exception("Could not open ROM file.");
    }
    else
    {
        size_t bytes = fread(buffer, sizeof(unsigned char), ROM_SIZE, filepoint);

        for (u32 i = currentAddress; i < bytes; i++)
        {
            memory.Write(i, buffer[i]);
        }

        fclose(filepoint);
    }

    RomLoaded = true;

    memory.Dump(0x0000, 0xFFFF);
}

void GameBoy::Render()
{
    gpu.Draw();
}

void GameBoy::Start()
{
    try
    {
        Address start = 0x8000;
        Address end = start + 0x4000;

        unsigned long long cycleCount = 0;

        this->cpu.StartCPU();

        while (this->cpu.IsRunning())
        {
            int cycles = this->cpu.Tick();

            cycleCount += cycles;

            //this->SimulateCycleDelay(cycles * CLOCK_CYCLES_PER_MACHINE_CYCLE);
            //std::this_thread::sleep_for(10ms);

            if (cycleCount % 1000 == 0)
            {
                gpu.LoadTileMap(start, end);
                this->Render();
            }
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