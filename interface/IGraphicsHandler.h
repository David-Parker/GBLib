#pragma once

#include "GlobalDefinitions.h"

// Handles the graphics data sent from the PPU. 
// Interface is injectable to GraphicsManager to allow for different rendering dependencies.
class IGraphicsHandler
{
public:
    // Initialize any dependenices. Called when LCD screen is powered on.
    virtual void Init() = 0;

    // Clear the screen (usually draw all white pixels to the screen).
    virtual void Clear() = 0;

    // Called from the ppu after all scanlines have been buffered.
    // pixelBuffer is an array of pixels in little endian ARGB8888 format.
    virtual void Draw(const u32* pixelBuffer, int width, int height) = 0;

    // Quit and release any resources for the graphics handler.
    virtual void Quit() = 0;
};