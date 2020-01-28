#include "GraphicsManager.h"

GraphicsManager::GraphicsManager()
{
}

GraphicsManager::~GraphicsManager()
{
}

void GraphicsManager::init()
{
    SDL_Init(SDL_INIT_VIDEO);
    SDL_CreateWindowAndRenderer(GraphicsManager::SCREEN_WIDTH, GraphicsManager::SCREEN_HEIGHT, 0, &window, &renderer);
}

void GraphicsManager::close()
{
    SDL_DestroyRenderer(renderer);
    renderer = nullptr;

    SDL_DestroyWindow(window);
    window = nullptr;

    SDL_Quit();
}

void GraphicsManager::Draw(int x, int y, SDL_Color color)
{
    Draw(x, y, color.r, color.g, color.b, color.a);
}

void GraphicsManager::Draw(int x, int y, int r, int g, int b, int a)
{
    int startx = x * SCALE;
    int starty = y * SCALE;
    SDL_SetRenderDrawColor(renderer, r, g, b, a);

    for (int i = 0; i < SCALE; i++)
    {
        for (int j = 0; j < SCALE; j++)
        {
            SDL_RenderDrawPoint(renderer, startx + i, starty + j);
        }
    }
}

void GraphicsManager::Flush()
{
    SDL_RenderPresent(renderer);
}

SDL_Color GraphicsManager::GetColor(unsigned int num)
{
    if (num > 3)
    {
        throw std::invalid_argument("Number exceeds range of color pallete.");
    }

    return ColorPallette[num];
}
