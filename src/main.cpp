#include <iostream>
#include <string>
#include "GameBoy.h"
#include "SDL.h"

int main(int argc, char* argv[])
{
    GameBoy* boy = new GameBoy();
    //boy->LoadRom("rom/Tetris.gb");
    //boy->LoadRom("rom/gb-test-roms/cpu_instrs/individual/01-special.gb");
    boy->LoadRom("rom/gb-test-roms/cpu_instrs/individual/02-interrupts.gb");

#ifdef _DEBUG
    boy->GetGameInfo().PrintInfo();
#endif

    boy->Start();

    return 0;
}