#include "GameBoy.h"
#include "CpuRelax.h"
#include <thread>

using namespace std::chrono;

GameBoy::GameBoy(std::string romFolder, IGraphicsHandler* graphicsHandler, IEventHandler* eventHandler, ISerialHandler* serialHandler, EMUType emuType)
    : memory(), devices(&memory, graphicsHandler, eventHandler, serialHandler), cpu(&memory, &devices.interruptController), romFolder(romFolder), graphicsHandler(graphicsHandler), eventHandler(eventHandler), serialHandler(serialHandler), romLoaded(false), mbc(nullptr), framesElapsed(0), emuType(emuType)
{
    this->MapIODevices();
    this->lastTimestamp = high_resolution_clock::now();
}

GameBoy::~GameBoy()
{
    delete this->bootROM;
    delete this->gameROM;
}

void GameBoy::LoadBootRom()
{
    // For CGB, loads the 2048-byte boot ROM into addresses 0x00 to 0x8FF
    if (IsCGB())
    {
        this->bootROM = new ROM(ADDR_BOOT_ROM_CGB_START, ADDR_BOOT_ROM_CGB_END);
        this->bootROM->LoadFromFile(this->romFolder + "/cgb_boot.bin", 0, 2048);

        this->memory.MapMemory(ADDR_BOOT_ROM_DMG_START, ADDR_BOOT_ROM_DMG_END, this->bootROM);
        this->memory.MapMemory(ADDR_BOOT_ROM_CGB_SECOND, ADDR_BOOT_ROM_CGB_END, this->bootROM);
    }
    // For DMG, loads the 256-byte boot ROM into addresses 0x00 to 0xFF
    else
    {
        this->bootROM = new ROM(ADDR_BOOT_ROM_DMG_START, ADDR_BOOT_ROM_DMG_END);
        this->bootROM->LoadFromFile(this->romFolder + "/dmg_boot.bin", 0, 256);
        this->memory.MapMemory(ADDR_BOOT_ROM_DMG_START, ADDR_BOOT_ROM_DMG_END, this->bootROM);
    }
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
        this->memory.MapMemory(ADDR_BOOT_ROM_DMG_END + 1, ADDR_GAME_ROM_END, this->gameROM);
        break;
    case CART_MBC1:
    case CART_MBC1_RAM:
    case CART_MBC1_RAM_BATTERY:
        this->mbc = new MBC1(this->cartridgeHeader);
        break;
    case CART_MBC2:
    case CART_MBC2_BATTERY:
        this->mbc = new MBC2(this->cartridgeHeader);
        break;
    case CART_MBC3:
    case CART_MBC3_RAM:
    case CART_MBC3_RAM_BATTERY:
    case CART_MBC3_TIMER_BATTERY:
    case CART_MBC3_TIMER_RAM_BATTERY:
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

        this->memory.MapMemory(ADDR_BOOT_ROM_DMG_END + 1, ADDR_GAME_ROM_END, this->mbc);
        this->memory.MapMemory(ADDR_EXTERNAL_RAM_START, ADDR_EXTERNAL_RAM_END, this->mbc);
    }

    // The emulator has elected to run in DMG or CGB based on the cartridge type.
    if (this->emuType == EMUType::Cartridge)
    {
        if (this->cartridgeHeader.isDMG)
        {
            this->emuType = EMUType::DMG;
        }
        else if (this->cartridgeHeader.isCGB)
        {
            this->emuType = EMUType::CGB;
        }
        else
        {
            throw std::runtime_error("Unsupported emulator type specified in the catridge header.");
        }
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

    u64 cycles = (u64)this->cpu.Tick() * CLOCK_CYCLES_PER_MACHINE_CYCLE;
    this->devices.ppu.Tick(cycles);
    this->devices.timerController.Tick(cycles);
    this->devices.serialController.Tick(cycles);
    this->cyclesElapsed += cycles;

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
    // Wait until the end of the next 1 / 60s (17ms) block.
    auto delta = high_resolution_clock::now() - this->lastTimestamp;
    auto waitTo = this->lastTimestamp + std::chrono::nanoseconds(CLOCK_NS_PER_FRAME);

    // Sleep is not very precise, only use if we have 1.5ms or more of waiting
    if (delta > std::chrono::microseconds(1500))
    {
        // Sleep
        std::this_thread::sleep_until(waitTo);
    }
    else
    {
        // Spin wait. This is much more precise than sleep, and allows us to emulate very large
        // speed multipliers such as 100x or 1000x. Power optimized using cpu_relax().
        while (high_resolution_clock::now() < waitTo)
            cpu_relax();
    }

    this->lastTimestamp = high_resolution_clock::now();
}