#pragma once
#include <stdexcept>
#include "SDL.h"

class GraphicsManager
{
private:
    SDL_Window *window;
    SDL_Renderer *renderer;
    SDL_Color ColorPallette[4] =
        { 
            SDL_Color(255, 255, 255, 0),
            SDL_Color(155, 215, 125, 0),
            SDL_Color(60, 140, 115, 0),
            SDL_Color(10, 50, 55, 0)
        };

    const static int SCALE = 4;
    const static int SCREEN_WIDTH = 256 * SCALE;
    const static int SCREEN_HEIGHT = 256 * SCALE;
public:
    GraphicsManager();
    ~GraphicsManager();

    void init();
    void close();
    void Draw(int x, int y, SDL_Color color);
    void Draw(int x, int y, int r, int g, int b, int a);
    void Flush();
    SDL_Color GetColor(unsigned int num);
};


