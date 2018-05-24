#include "GameInfo.h"
#include <iostream>

GameInfo::GameInfo()
{
}

GameInfo::~GameInfo()
{
}

void GameInfo::PrintInfo()
{
	std::cout << "Title: " << title << std::endl;
}
