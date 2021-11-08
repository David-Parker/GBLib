#include "GameBoy.h"
#include <thread>

using namespace std::chrono;

GameBoy::GameBoy(std::string romFolder, IGraphicsHandler* graphicsHandler, IEventHandler* eventHandler)
    : memory(), devices(&memory, graphicsHandler, eventHandler), cpu(&memory, &devices.interruptController), romFolder(romFolder), graphicsHandler(graphicsHandler), eventHandler(eventHandler), romLoaded(false), mbc(nullptr), framesElapsed(0)
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
    this->bootROM = new ROM(ADDR_BOOT_ROM_START, ADDR_BOOT_ROM_END);
    this->bootROM->LoadFromFile(this->romFolder + "/boot.bin", 0, 256);
    this->memory.MapMemory(ADDR_BOOT_ROM_START, ADDR_BOOT_ROM_END, this->bootROM);
}

// Loads the ROM file into memory
void GameBoy::LoadRom(std::string path)
{
    std::string gamePath = this->romFolder + "/" + path;

    // Loads the cartridge header information.
    this->cartridgeHeader.Read(gamePath);
    this->cartridgeHeader.PrintInfo();

    // Game ROM 0x00 to 0xFF is mapped after boot sequence is completed.
    switch (this->cartridgeHeader.cartridgeType)
    {
    case CART_ROM_ONLY:
        this->gameROM = new ROM(ADDR_GAME_ROM_START, ADDR_GAME_ROM_END);
        this->gameROM->LoadFromFile(gamePath, 0, (size_t)this->cartridgeHeader.NumROMBanks() * ROM_BANK_BYTES);
        this->memory.MapMemory(ADDR_BOOT_ROM_END + 1, ADDR_GAME_ROM_END, this->gameROM);
        break;
    case CART_MBC1:
    case CART_MBC1_RAM:
    case CART_MBC1_RAM_BATTERY:
        this->mbc = new MBC1(this->cartridgeHeader);
        break;
    case CART_MBC3:
    case CART_MBC3_RAM:
    case CART_MBC3_RAM_BATTERY:
        this->mbc = new MBC3(this->cartridgeHeader);
        break;
    default:
        throw std::runtime_error("Cartridge type not supported.");
    }

    if (this->mbc != nullptr)
    {
        this->mbc->LoadROMFromFile(gamePath);

        if (this->cartridgeHeader.HasBattery())
        {
            this->mbc->LoadRAMFromSave(this->romFolder+ "/saves/" + this->cartridgeHeader.title + ".save");
        }

        this->memory.MapMemory(ADDR_BOOT_ROM_END + 1, ADDR_GAME_ROM_END, this->mbc);
        this->memory.MapMemory(ADDR_EXTERNAL_RAM_START, ADDR_EXTERNAL_RAM_END, this->mbc);
    }

    LoadBootRom();

    this->romLoaded = true;
}

CartridgeHeader GameBoy::GetCartridgeHeader()
{
    if (!this->romLoaded)
    {
        throw std::runtime_error("Cannot read cartridge header, ROM has not been loaded.");
    }

    return this->cartridgeHeader;
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

bool GameBoy::ShouldStop()
{
    return this->eventHandler->ShouldQuit();
}

void GameBoy::Start()
{
    this->cpu.StartCPU();
    this->cyclesElapsed = 0;
    this->framesElapsed = 0;
}

void GameBoy::Stop()
{
    this->cpu.StopCPU();
    this->graphicsHandler->Quit();
}

int GameBoy::Step()
{
    if (!this->cpu.IsRunning())
    {
        throw std::runtime_error("Cannot step a stopped CPU.");
    }

    int cycles = this->cpu.Tick();
    this->devices.ppu.Tick(cycles);
    this->devices.timerController.Tick(cycles);
    this->cyclesElapsed += (u64)cycles * CLOCK_CYCLES_PER_MACHINE_CYCLE;

    return cycles;
}

void GameBoy::Run()
{
    while (!ShouldStop())
    {
        Step();

        int framesDelta = FramesElapsed() - framesElapsed;

        // Another frame has elapsed.
        if (framesDelta >= this->eventHandler->SpeedMultiplier())
        {
            framesElapsed += framesDelta;
            SimulateFrameDelay();
        }
    }
}

void GameBoy::SaveGame()
{
    if (this->cartridgeHeader.HasBattery())
    {
        this->mbc->SaveToFile(this->romFolder + "/saves/" + this->cartridgeHeader.title + ".save");
    }
}

u64 GameBoy::FramesElapsed()
{
    return this->cyclesElapsed / CLOCK_CYCLES_PER_FRAME;
}

void GameBoy::SimulateFrameDelay()
{
    // After one frame has passed (16.7ms / 69905 clocks) wait till the end of the "real time" for that frame
    auto delta = high_resolution_clock::now() - this->lastTimestamp;
    auto waitTo = this->lastTimestamp + std::chrono::nanoseconds(CLOCK_NS_PER_FRAME);

    if (delta > std::chrono::milliseconds(10))
    {
        // Sleep
        std::this_thread::sleep_until(waitTo);
    }
    else
    {
        // Spin wait
        while (high_resolution_clock::now() < waitTo);
    }

    this->lastTimestamp = high_resolution_clock::now();
}