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
    boy.Render();

    while (1)
    {
        int cycles = boy.TickCpu();
        boy.SimulateCycleDelay(cycles * CLOCK_CYCLES_PER_MACHINE_CYCLE);
    }
}