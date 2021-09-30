#pragma once
#include "SDL.h"
#include "Tile.h"
#include <stdexcept>
#include <vector>

#define DMG_COLOR_BLACK SDL_Color(0, 0, 0, 255)
#define DMG_COLOR_LIGHT_GRAY SDL_Color(155, 155, 155, 155)
#define DMG_COLOR_DARK_GRAY SDL_Color(55, 55, 55, 255)
#define DMG_COLOR_WHITE SDL_Color(255, 255, 255, 255)

class GraphicsManager
{
private:
    SDL_Window *window;
    SDL_Renderer *renderer;
    SDL_Texture* texture;

    SDL_Color ColorPalette[4] =
    {
        DMG_COLOR_BLACK,
        DMG_COLOR_LIGHT_GRAY,
        DMG_COLOR_DARK_GRAY,
        DMG_COLOR_WHITE
    };

    u32 EncodedPalette[4] =
    {
        EncodeColor(0),
        EncodeColor(1),
        EncodeColor(2),
        EncodeColor(3)
    };

    u32 pixelBuffer[SCREEN_HEIGHT][SCREEN_WIDTH];

    u32 EncodeColor(Byte index);
public:
    GraphicsManager();
    ~GraphicsManager();

    void Init();
    void Close();
    void SetPalette(Byte index, SDL_Color color);
    void ReloadPalette();
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