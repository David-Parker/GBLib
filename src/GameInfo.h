#pragma once
#include "GlobalDefinitions.h"
#include "Memory.h"
#include <string>
class GameInfo
{
public:
    std::string title;
    bool isCGB; // GameBoy Color
	bool isSGB; // Super GameBoy
	u8 cartridge_type;

	char* cartridge_types[256];

    GameInfo();
    ~GameInfo();
	void Read(Memory* pMemory);
    void PrintInfo();
};

