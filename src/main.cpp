#include <iostream>
#include <string>
#include "GameBoy.h"
#include "SDLGraphicsHandler.h"
#include "SDLEventHandler.h"
#include "SDL.h"

int main(int argc, char* argv[])
{
    GameBoy* boy = new GameBoy(
        "rom/save",
        new SDLGraphicsHandler(SCALED_SCREEN_WIDTH, SCALED_SCREEN_HEIGHT), 
        new SDLEventHandler());

    // Inject SDL based handlers for desktop builds.
    try
    {
        boy->LoadRom("rom/gb-test-roms/cpu_instrs/cpu_instrs.gb");
        boy->Start();
    }
    catch (std::exception& ex)
    {
        boy->Stop();
        std::cout << "Error detected. " << ex.what() << std::endl;
    }

    return 0;
}