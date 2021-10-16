#include "GraphicsManager.h"
#include "SDL_events.h"

GraphicsManager::GraphicsManager(int width, int height, int scale)
    : width(width), height(height), scale(scale)
{
    this->pixelBuffer = (u32*)malloc(this->width * this->height * sizeof(u32));
}

GraphicsManager::~GraphicsManager()
{
    free( this->pixelBuffer);
}

u32 GraphicsManager::EncodeColor(Byte index)
{
    SDL_Color color = ColorPalette[index];

    u32 result = color.a;
    result <<= 8;
    result += color.r;
    result <<= 8;
    result += color.g;
    result <<= 8;
    result += color.b;

    return result;
}

void GraphicsManager::SetPalette(Byte index, SDL_Color color)
{
    ColorPalette[index] = color;
}

void GraphicsManager::ReloadPalette()
{
    for (int i = 0; i < 4; ++i)
    {
        EncodedPalette[i] = EncodeColor(i);
    }
}

void GraphicsManager::AddPixel(int x, int y, Byte color)
{
    int startx = x * this->scale;
    int starty = y * this->scale;

    for (int i = 0; i < this->scale; i++)
    {
        for (int j = 0; j < this->scale; j++)
        {
            int idx = (starty+i) * width + (startx + j);
            this->pixelBuffer[idx] = EncodedPalette[color];
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

    texture = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_ARGB8888, SDL_TEXTUREACCESS_STREAMING, this->width, this->height);
}

void GraphicsManager::Close()
{
    SDL_DestroyRenderer(renderer);
    renderer = nullptr;

    SDL_DestroyWindow(window);
    window = nullptr;

    SDL_DestroyTexture(texture);
    texture = nullptr;
}

void GraphicsManager::Clear()
{
    SDL_RenderClear(renderer);
}

void GraphicsManager::Draw()
{
    void* pixels;
    int pitch;

    SDL_LockTexture(texture, NULL, &pixels, &pitch);

    memcpy(pixels, pixelBuffer, this->width * this->height * sizeof(u32));

    SDL_UnlockTexture(texture);

    SDL_RenderCopy(renderer, texture, NULL, NULL);
}

void GraphicsManager::Flush()
{
    SDL_RenderPresent(renderer);
}

void GraphicsManager::HandleEvents()
{
    SDL_Event event;

    while (SDL_PollEvent(&event)) { 
        if (event.type == SDL_QUIT)
        {
            SDL_Quit();
            exit(EXIT_SUCCESS);
        }
    }
}