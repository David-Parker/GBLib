#pragma once
#include <string>
#include <errno.h>
#include <stdexcept>
#include "Memory.h"
#include "GameInfo.h"
#include "Cpu.h"
#include "Gpu.h"

class GameBoy
{
private:
	static const int ROM_SIZE = 0xFFFF;
	bool RomLoaded = false;
	Gpu gpu;
	Cpu cpu;
public:
	Memory memory;
	GameBoy() : memory() {}
	void LoadRom(std::string path);
	GameInfo GetGameInfo();
};