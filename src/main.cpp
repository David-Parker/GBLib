#include <iostream>
#include <string>
#include "GameBoy.h"
#include "SDLGraphicsHandler.h"
#include "SDLEventHandler.h"
#include "SDL.h"

int main(int argc, char* argv[])
{
    GameBoy* boy = new GameBoy(
        new SDLGraphicsHandler(SCALED_SCREEN_WIDTH, SCALED_SCREEN_HEIGHT), 
        new SDLEventHandler());

    // Inject SDL based handlers for desktop builds.
    try
    {
        //boy->LoadRom("rom/Dr. Mario.gb");
        //boy->LoadRom("rom/Tetris.gb");
        //boy->LoadRom("rom/Tennis.gb");
        boy->LoadRom("rom/Super Mario Land.gb");
        //boy->LoadRom("rom/F-1 Race.gb");
        //boy->LoadRom("rom/Pokemon.gb");
        //boy->LoadRom("rom/gb-test-roms/cpu_instrs/cpu_instrs.gb");
        //boy->LoadRom("rom/gb-test-roms/interrupt_time/interrupt_time.gb");
        //boy->LoadRom("rom/gb-test-roms/halt_bug.gb");

        boy->Start();
    }
    catch (std::exception& ex)
    {
        boy->Stop();
        std::cout << "Error detected. " << ex.what() << std::endl;
    }

    return 0;
}