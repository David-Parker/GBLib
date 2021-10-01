#include <iostream>
#include <string>
#include "GameBoy.h"

#undef main

int main()
{
    GameBoy* boy = new GameBoy();
    boy->LoadRom("rom/Tetris.gb");
    //boy->LoadTestRom();
    boy->GetGameInfo().PrintInfo();
    boy->Start();

    return 0;
}