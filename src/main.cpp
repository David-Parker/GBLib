#include <iostream>
#include <string>
#include "GameBoy.h"
#include "SDLGraphicsHandler.h"
#include "SDLEventHandler.h"
#include "SDL.h"

int main(int argc, char* argv[])
{
    // Inject SDL based handlers for desktop builds.
    GameBoy* boy = new GameBoy(new SDLGraphicsHandler(SCALED_SCREEN_WIDTH, SCALED_SCREEN_HEIGHT), new SDLEventHandler());

    //boy->LoadRom("rom/Dr. Mario.gb");
    boy->LoadRom("rom/Tetris.gb");
    //boy->LoadRom("rom/Tennis.gb");
    //boy->LoadRom("rom/Super Mario Land.gb");
    //boy->LoadRom("rom/F-1 Race.gb");
    //boy->LoadRom("rom/Pokemon.gb");
    //boy->LoadRom("rom/gb-test-roms/cpu_instrs/individual/01-special.gb"); // Passed
    //boy->LoadRom("rom/gb-test-roms/cpu_instrs/individual/02-interrupts.gb"); // Passed
    //boy->LoadRom("rom/gb-test-roms/cpu_instrs/individual/03-op sp,hl.gb"); // Passed
    //boy->LoadRom("rom/gb-test-roms/cpu_instrs/individual/04-op r,imm.gb"); // Passed
    //boy->LoadRom("rom/gb-test-roms/cpu_instrs/individual/05-op rp.gb"); // Passed
    //boy->LoadRom("rom/gb-test-roms/cpu_instrs/individual/06-ld r,r.gb"); // Passed
    //boy->LoadRom("rom/gb-test-roms/cpu_instrs/individual/07-jr,jp,call,ret,rst.gb"); // Passed
    //boy->LoadRom("rom/gb-test-roms/cpu_instrs/individual/08-misc instrs.gb"); // Passed
    //boy->LoadRom("rom/gb-test-roms/cpu_instrs/individual/09-op r,r.gb"); // Passed
    //boy->LoadRom("rom/gb-test-roms/cpu_instrs/individual/10-bit ops.gb"); // Passed
    //boy->LoadRom("rom/gb-test-roms/cpu_instrs/individual/11-op a,(hl).gb"); // Passed

    boy->Start();

    return 0;
}