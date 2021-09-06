#include <iostream>
#include <string>
#include "GameBoy.h"
#include "Gpu.h"

#undef main

int main()
{
	GameBoy* boy = new GameBoy();
    boy->LoadRom("rom/Tetris.gb");
	//boy->LoadTestRom();
	boy->GetGameInfo().PrintInfo();
	boy->Start();
}