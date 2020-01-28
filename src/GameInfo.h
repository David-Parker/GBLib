#pragma once
#include <string>
class GameInfo
{
public:
    std::string title;
    bool isGBColor;

    GameInfo();
    ~GameInfo();
    void PrintInfo();
};

