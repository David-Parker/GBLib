#include <iostream>
#include <string>
#include "GameBoy.h"
#include "Gpu.h"

#undef main

int main()
{
    GameBoy boy;
    boy.LoadRom("rom/Tetris.gb");
    boy.GetGameInfo().PrintInfo();
	boy.Start();
}