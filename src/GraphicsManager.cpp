#include "GraphicsManager.h"
#include "SDL_events.h"

GraphicsManager::GraphicsManager(int width, int height, int scale, int layers)
    : width(width), height(height), scale(scale), layers(layers)
{
    size_t bytes = width * height * sizeof(u32);
    this->transparentBuffer = (u32*)malloc(width * height * sizeof(u32));
    u32 color = EncodeColor(TRANSPARENT_BACKGROUND);

    for (int i = 0; i < width * height; ++i)
    {
        this->transparentBuffer[i] = color;
    }
}

GraphicsManager::~GraphicsManager()
{
    if (renderer != nullptr)
        SDL_DestroyRenderer(renderer);

    if (window != nullptr)
        SDL_DestroyWindow(window);

    free(this->transparentBuffer);

    for (int i = 0; i < this->layers.size(); ++i)
    {
        free(this->layers[i].pixelBuffer);

        if (this->layers[i].texture != nullptr)
        {
            SDL_DestroyTexture(this->layers[i].texture);
        }
    }
}

u32 GraphicsManager::EncodeColor(SDL_Color color)
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
    if (SDL_Init(SDL_INIT_VIDEO) != 0)
    {
        throw std::exception("Error: Failed to initialize SDL.");
    }

    if (SDL_CreateWindowAndRenderer(this->width, this->height, SDL_WINDOW_SHOWN, &window, &renderer) != 0)
    {
        throw std::exception("Error: Failed to new SDL window.");
    }

    SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);

    for (int i = 0; i < this->layers.size(); ++i)
    {
        this->layers[i].pixelBuffer = (u32*)malloc(width * height * sizeof(u32));
        this->layers[i].texture = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_ARGB8888, SDL_TEXTUREACCESS_STREAMING, this->width, this->height);
        SDL_SetTextureBlendMode(this->layers[i].texture, SDL_BLENDMODE_BLEND);
        memcpy(this->layers[i].pixelBuffer, this->transparentBuffer, this->width * this->height * sizeof(u32));
    }
}

void GraphicsManager::Clear()
{
    SDL_RenderClear(renderer);
}

void GraphicsManager::Draw()
{
    void* pixels;
    int pitch;

    for (int i = 0; i < this->layers.size(); ++i)
    {
        GraphicsLayer* layer = &this->layers[i];

        SDL_LockTexture(layer->texture, NULL, &pixels, &pitch);

        size_t bytes = this->width * this->height * sizeof(u32);
        memcpy(pixels, layer->pixelBuffer, bytes);
        memcpy(layer->pixelBuffer, this->transparentBuffer, bytes);

        SDL_UnlockTexture(layer->texture);

        SDL_RenderCopy(renderer, layer->texture, NULL, NULL);
    }
}

void GraphicsManager::Flush()
{
    SDL_RenderPresent(renderer);
}