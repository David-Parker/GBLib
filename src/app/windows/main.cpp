#include <iostream>
#include <string>
#include "GameBoy.h"
#include "SDLGraphicsHandler.h"
#include "SDLEventHandler.h"
#include "SDL.h"

int main(int argc, char* argv[])
{
    std::string romPath;

    std::cout << "Enter the path to your ROM file, e.g. rom/Pokemon.gb" << std::endl;
    std::cin >> romPath;

    // Inject SDL based handlers for desktop builds.
    GameBoy* boy = new GameBoy(
        "rom/save",
        new SDLGraphicsHandler(SCALED_SCREEN_WIDTH, SCALED_SCREEN_HEIGHT), 
        new SDLEventHandler());

    try
    {
        boy->LoadRom(romPath);
        boy->Start();
    }
    catch (std::exception& ex)
    {
        boy->Stop();
        std::cout << "Error detected. " << ex.what() << std::endl;
    }

    return 0;
}