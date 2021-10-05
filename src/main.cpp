#include <iostream>
#include <string>
#include "GameBoy.h"
#include "SDL.h"

int main(int argc, char* argv[])
{
    GameBoy* boy = new GameBoy();
    boy->LoadRom("rom/Tetris.gb");
    //boy->LoadRom("rom/tests/cpu_instrs.gb");
    boy->GetGameInfo().PrintInfo();
    boy->Start();

    return 0;
}