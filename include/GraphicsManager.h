#pragma once
#include "IGraphicsHandler.h"
#include <stdexcept>
#include <vector>

#define DMG_COLOR_BLACK Color(0, 0, 0, 255)
#define DMG_COLOR_LIGHT_GRAY Color(155, 155, 155, 255)
#define DMG_COLOR_DARK_GRAY Color(55, 55, 55, 255)
#define DMG_COLOR_WHITE Color(255, 255, 255, 255)

struct Color
{
    u8 r;
    u8 g;
    u8 b;
    u8 a;

    Color(u8 _r, u8 _g, u8 _b, u8 _a) : r(_r), g(_g), b(_b), a(_a) {}
};

struct GraphicsLayer
{
    u32* pixelBuffer;

    GraphicsLayer()
        : pixelBuffer(nullptr)
    {}
};

class GraphicsManager
{
private:
    int width, height, numLayers;
    GraphicsLayer* layers;
    IGraphicsHandler* graphicsHandler;
    u32* flattenedLayers;

    Color ColorPalette[4] =
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

    u32 EncodeColor(Color color);
public:
    GraphicsManager(IGraphicsHandler* graphicsHandler, int width, int height, int layers);
    ~GraphicsManager();

    void Init();
    void AddPixel(int x, int y, Byte color, Byte palette[4], int layer);
    void Clear();
    void Draw();
};