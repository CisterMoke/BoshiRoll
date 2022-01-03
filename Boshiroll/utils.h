#pragma once
#include <iostream>
#include <string>
#include<SDL.h>
#include<SDL_image.h>
#include<SDL2_gfxPrimitives.h>
#include "globals.h"
#include "Colliders.h"

SDL_Surface *loadSurface(std::string path, SDL_PixelFormatEnum pixelFormat = SDL_PIXELFORMAT_RGB888);

SDL_Texture *loadTexture(std::string path, SDL_PixelFormatEnum pixelFormat = SDL_PIXELFORMAT_RGB888);
