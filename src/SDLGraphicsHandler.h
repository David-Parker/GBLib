#pragma once
#include "IGraphicsHandler.h"
#include "SDL.h"

class SDLGraphicsHandler :
    public IGraphicsHandler
{
private:
    int width, height;
    SDL_Window* window;
    SDL_Renderer* renderer;
    SDL_Texture* texture;

public:
    SDLGraphicsHandler(int width, int height);
    ~SDLGraphicsHandler();

    void Init();
    void Clear();
    void Draw(u32* pixelBuffer, int width, int height, int layer);
    void Flush();
};