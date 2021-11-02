#pragma once

#include "GlobalDefinitions.h"

// Handles the graphics data sent from the PPU. Interface is injectable to GraphicsManager to allow for different rendering dependencies.
class IGraphicsHandler
{
public:

    // Initialize any dependenices. Called when LCD screen is powered on.
    virtual void Init() = 0;

    // Clear the screen (usually draw all white pixels to the screen).
    virtual void Clear() = 0;

    // Called from the ppu for each layer (bg, window, sprites), pixelBuffer is an array of pixels in little endian ARGB8888 format.
    // If your machine is big endian, format will be BGRA8888.
    virtual void Draw(const u32* pixelBuffer, int width, int height) = 0;

    // Flush all buffered layers to final output. Called once all layers have requested to be drawn.
    virtual void Flush() = 0;

    // Quit and release any resources for the graphics handler.
    virtual void Quit() = 0;
};