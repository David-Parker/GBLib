#include "GameBoy.h"

void GameBoy::LoadRom(std::string path)
{
	// Clear memory
	memory.ClearMemory();

	unsigned currentAddress = 0x0; // Game Code starts at address 100, load scrolling graphic at 104
	unsigned char buffer[ROM_SIZE];
	FILE *filepoint;
	errno_t err;

	if ((err = fopen_s(&filepoint, path.c_str(), "rb")) != 0) {
		throw std::exception("Could not open ROM file.");
	}
	else {
		fread(buffer, sizeof(unsigned char), ROM_SIZE, filepoint);

		for (int i = 0; i < ROM_SIZE; i++)
		{
			memory.LoadValue(i, buffer[i]);
		}

		fclose(filepoint);
	}

	RomLoaded = true;

	memory.MemoryDump(0x0000, 0xFFFF);
}

GameInfo GameBoy::GetGameInfo()
{
	if (!RomLoaded)
	{
		throw new std::exception("Can not get GameInfo. No ROM is loaded.");
	}

	// Get the title
	char title[16] = {};
	int loc = 0;
	for (int i = 0x134; i <= 0x142; ++i)
	{
		Byte b = memory.ReadValue(i);
		title[loc++] = memory.ReadValue(i);
	}

	std::string titleName(title);

	GameInfo info;
	info.title = titleName;

	return info;
}
