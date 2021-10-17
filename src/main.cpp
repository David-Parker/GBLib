#include <iostream>
#include <string>
#include "GameBoy.h"
#include "SDL.h"

int main(int argc, char* argv[])
{
    GameBoy* boy = new GameBoy();
    boy->LoadRom("rom/TetrisV1.gb");
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

#ifdef _DEBUG
    boy->GetGameInfo().PrintInfo();
#endif

    boy->Start();

    return 0;
}