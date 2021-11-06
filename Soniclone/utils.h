#pragma once
#include <iostream>
#include <string>
#include<SDL.h>
#include<SDL_image.h>
#include<SDL2_gfxPrimitives.h>
#include "globals.h"
#include "LineCollider.h"
#include "CircleCollider.h"
#include "RectCollider.h"

SDL_Surface *loadSurface(std::string path, SDL_PixelFormatEnum pixelFormat = SDL_PIXELFORMAT_RGB888);

SDL_Texture *loadTexture(std::string path, SDL_PixelFormatEnum pixelFormat = SDL_PIXELFORMAT_RGB888);

namespace debug
{
	void drawCollider(CircleCollider const &c, SDL_Color const &color);
	void drawCollider(RectCollider const &r, SDL_Color const &color);
	void drawCollider(LineCollider const &r, SDL_Color const &color);
}

