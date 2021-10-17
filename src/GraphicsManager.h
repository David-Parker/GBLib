#pragma once
#include "SDL.h"
#include "Tile.h"
#include <stdexcept>
#include <vector>

#define DMG_COLOR_BLACK SDL_Color(0, 0, 0, 255)
#define DMG_COLOR_LIGHT_GRAY SDL_Color(155, 155, 155, 255)
#define DMG_COLOR_DARK_GRAY SDL_Color(55, 55, 55, 255)
#define DMG_COLOR_WHITE SDL_Color(255, 255, 255, 255)
#define TRANSPARENT_BACKGROUND SDL_Color(0, 0, 0, 0)

struct GraphicsLayer
{
    SDL_Texture* texture;
    u32* pixelBuffer;

    GraphicsLayer()
        : texture(nullptr), pixelBuffer(nullptr)
    {}
};

class GraphicsManager
{
private:
    int width, height, scale, numLayers;
    SDL_Window *window;
    SDL_Renderer *renderer;
    GraphicsLayer* layers;
    u32* transparentBuffer;
    u32 transparentEncoded;

    SDL_Color ColorPalette[4] =
    {
        DMG_COLOR_WHITE,
        DMG_COLOR_LIGHT_GRAY,
        DMG_COLOR_DARK_GRAY,
        DMG_COLOR_BLACK
    };

    u32 EncodedPalette[4] =
    {
        EncodeColor(ColorPalette[0]),
        EncodeColor(ColorPalette[1]),
        EncodeColor(ColorPalette[2]),
        EncodeColor(ColorPalette[3])
    };

    u32 EncodeColor(SDL_Color color);
public:
    GraphicsManager(int width, int height, int scale, int layers);
    ~GraphicsManager();

    void Init();
    void AddPixel(int x, int y, Byte color, Byte palette[4], int layer, bool checkTransparent);
    void Clear();
    void Draw();
    void Flush();
};