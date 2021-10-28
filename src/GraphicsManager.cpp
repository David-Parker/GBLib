#include "GraphicsManager.h"
#include <cstring>

GraphicsManager::GraphicsManager(IGraphicsHandler* graphicsHandler, int width, int height, int scale, int numLayers)
    : graphicsHandler(graphicsHandler), width(width), height(height), scale(scale), numLayers(numLayers)
{
    size_t bytes = width * height * sizeof(u32);
    this->transparentBuffer = (u32*)malloc(width * height * sizeof(u32));
    this->transparentEncoded = EncodeColor(TRANSPARENT_BACKGROUND);
    this->layers = (GraphicsLayer*)malloc(sizeof(GraphicsLayer) * numLayers);

    for (int i = 0; i < width * height; ++i)
    {
        this->transparentBuffer[i] = this->transparentEncoded;
    }
}

GraphicsManager::~GraphicsManager()
{
    free(this->transparentBuffer);

    for (int i = 0; i < this->numLayers; ++i)
    {
        free(this->layers[i].pixelBuffer);
    }

    free(this->layers);
}

u32 GraphicsManager::EncodeColor(Color color)
{
    u32 result = color.a;
    result <<= 8;
    result += color.r;
    result <<= 8;
    result += color.g;
    result <<= 8;
    result += color.b;

    return result;
}

void GraphicsManager::AddPixel(int x, int y, Byte color, Byte palette[4], int layer)
{
    int startx = x * this->scale;
    int starty = y * this->scale;

    for (int i = 0; i < this->scale; i++)
    {
        for (int j = 0; j < this->scale; j++)
        {
            int idx = (starty+i) * width + (startx + j);

            this->layers[layer].pixelBuffer[idx] = EncodedPalette[palette[color]];
        }
    }
}

void GraphicsManager::Init()
{
    for (int i = 0; i < this->numLayers; ++i)
    {
        this->layers[i].pixelBuffer = (u32*)malloc(width * height * sizeof(u32));
        std::memcpy(this->layers[i].pixelBuffer, this->transparentBuffer, this->width * this->height * sizeof(u32));
    }

    this->graphicsHandler->Init();
}

void GraphicsManager::Clear()
{
    this->graphicsHandler->Clear();
}

void GraphicsManager::Draw()
{
    for (int i = 0; i < this->numLayers; ++i)
    {
        GraphicsLayer* layer = &this->layers[i];

        size_t bytes = this->width * this->height * sizeof(u32);
        this->graphicsHandler->Draw(layer->pixelBuffer, this->width, this->height, i);
        std::memcpy(layer->pixelBuffer, this->transparentBuffer, bytes);
    }

    this->graphicsHandler->Flush();
}