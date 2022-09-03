#pragma once
#include <iostream>
#include <string>
#include<SDL.h>
#include<SDL_image.h>
#include<SDL2_gfxPrimitives.h>
#include "globals.h"


SDL_Surface *load_surface(std::string path, SDL_PixelFormatEnum pixelFormat = SDL_PIXELFORMAT_RGB888);

SDL_Texture *load_texture(std::string path, SDL_PixelFormatEnum pixelFormat = SDL_PIXELFORMAT_RGB888);

std::string debug_mode_string();
