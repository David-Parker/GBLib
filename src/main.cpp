#include <iostream>
#include <string>
#include "GameBoy.h"

#undef main

int main()
{
    GameBoy* boy = new GameBoy();
    boy->LoadRom("rom/Tetris.gb");
    //boy->LoadRom("rom/tests/cpu_instrs.gb");
    boy->GetGameInfo().PrintInfo();
    boy->Start();

    return 0;
}