#include "SDLGraphicsHandler.h"
#include <stdexcept>

SDLGraphicsHandler::SDLGraphicsHandler(int width, int height)
    : width(width), height(height)
{
}

SDLGraphicsHandler::~SDLGraphicsHandler()
{
    if (this->renderer != nullptr)
        SDL_DestroyRenderer(this->renderer);

    if (this->window != nullptr)
        SDL_DestroyWindow(this->window);

    if (this->texture != nullptr)
        SDL_DestroyTexture(this->texture);
}

void SDLGraphicsHandler::Init()
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
    this->texture = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_ARGB8888, SDL_TEXTUREACCESS_STREAMING, this->width, this->height);
    SDL_SetTextureBlendMode(this->texture, SDL_BLENDMODE_BLEND);
}

void SDLGraphicsHandler::Clear()
{
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    SDL_RenderClear(renderer);
    SDL_RenderPresent(renderer);
}

void SDLGraphicsHandler::Draw(u32* pixelBuffer, int width, int height, int layer)
{
    void* pixels;
    int pitch;

    size_t bytes = this->width * this->height * sizeof(u32);

    SDL_LockTexture(this->texture, NULL, &pixels, &pitch);
    memcpy(pixels, pixelBuffer, bytes);
    SDL_UnlockTexture(this->texture);
    SDL_RenderCopy(renderer, this->texture, NULL, NULL);
}

void SDLGraphicsHandler::Flush()
{
    SDL_RenderPresent(renderer);
}