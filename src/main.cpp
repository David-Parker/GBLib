#include <iostream>
#include <string>
#include "GameBoy.h"

int main(int argc, char* args[])
{
    GameBoy* boy = new GameBoy();
    boy->LoadRom("rom/Tetris.gb");
    //boy->LoadTestRom();
    boy->GetGameInfo().PrintInfo();
    boy->Start();

    return 0;
}