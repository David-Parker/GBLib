#include "GraphicsManager.h"
#include <cstring>

GraphicsManager::GraphicsManager(IGraphicsHandler* graphicsHandler, int width, int height, int numLayers)
    : graphicsHandler(graphicsHandler), width(width), height(height), numLayers(numLayers)
{
    size_t bytes = (u64)width * height * sizeof(u32);
    this->flattenedLayers = (u32*)malloc((u64)width * height * sizeof(u32));
    this->layers = (GraphicsLayer*)malloc(sizeof(GraphicsLayer) * numLayers);
}

GraphicsManager::~GraphicsManager()
{
    free(this->flattenedLayers);

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
    int idx = (y * width) + x;

    this->layers[layer].pixelBuffer[idx] = EncodedPalette[palette[color]];
}

void GraphicsManager::Init()
{
    for (int i = 0; i < this->numLayers; ++i)
    {
        this->layers[i].pixelBuffer = (u32*)malloc((size_t)width * height * sizeof(u32));

        if (this->layers[i].pixelBuffer == nullptr)
        {
            throw std::runtime_error("No memory available for the pixel buffer.");
        }

        std::memset(this->layers[i].pixelBuffer, 0, (size_t)this->width * this->height * sizeof(u32));
    }

    this->graphicsHandler->Init();
}

void GraphicsManager::Clear()
{
    this->graphicsHandler->Clear();
}

void GraphicsManager::Draw()
{
    // Flatten all layers into a single buffer.
    for (int i = 0; i < this->numLayers; ++i)
    {
        GraphicsLayer* layer = &this->layers[i];

        for (int j = 0; j < this->width * this->height; ++j)
        {
            if (layer->pixelBuffer[j] == 0)
            {
                continue;
            }

            this->flattenedLayers[j] = layer->pixelBuffer[j];
        }

        std::memset(layer->pixelBuffer, 0, (size_t)this->width * this->height * sizeof(u32));
    }

    this->graphicsHandler->Draw(this->flattenedLayers, this->width, this->height);
}