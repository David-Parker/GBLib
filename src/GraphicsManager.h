#pragma once
#include "SDL.h"
#include "Tile.h"
#include <stdexcept>
#include <vector>

class GraphicsManager
{
private:
    SDL_Window *window;
    SDL_Renderer *renderer;
    SDL_Texture* texture;

    SDL_Color ColorPallette[4] =
    {
        SDL_Color(0, 0, 0, 0),
        SDL_Color(155, 215, 125, 255),
        SDL_Color(60, 140, 115, 255),
        SDL_Color(10, 50, 55, 255)
    };

    u32 EncodedPalette[4] =
    {
        EncodeColor(0),
        EncodeColor(1),
        EncodeColor(2),
        EncodeColor(3)
    };

    u32 pixelBuffer[SCREEN_HEIGHT][SCREEN_WIDTH];

    SDL_Color GetColor(Byte num);
    u32 EncodeColor(Byte index);
public:
    GraphicsManager();
    ~GraphicsManager();

    void Init();
    void Close();
    void AddPixel(int x, int y, Byte color);
    void Clear();
    void Draw();
    void Flush();
};

inline void GraphicsManager::AddPixel(int x, int y, Byte color)
{
    int startx = x * SCALE;
    int starty = y * SCALE;

    for (int i = 0; i < SCALE; i++)
    {
        for (int j = 0; j < SCALE; j++)
        {
            pixelBuffer[starty + j][startx + i] = EncodedPalette[color];
        }
    }
}