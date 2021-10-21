#include "BaseSprite.h"
using glob::gRenderer;

BaseSprite::BaseSprite()
{
	texture = NULL;
	baseSurf = NULL;
	w = 0;
	h = 0;
	scale_x = 1;
	scale_y = 1;
};

BaseSprite::~BaseSprite()
{
	clearMem();
}

bool BaseSprite::loadFromFile(std::string path, int mode)
{
	reset();

	baseSurf = loadSurface(path, SDL_PIXELFORMAT_RGBA8888);
	if (baseSurf == NULL)
	{
		return false;
	}

	w = baseSurf->w;
	h = baseSurf->h;

	if (!(mode & 0x01)) { baseSurf = SDL_ConvertSurfaceFormat(baseSurf, SDL_PIXELFORMAT_RGB888, 0); }
	if (mode & 0x02) { SDL_SetColorKey(baseSurf, SDL_TRUE, SDL_MapRGB(baseSurf->format, 0xFF, 0x0, 0xFF)); }
	texture = SDL_CreateTextureFromSurface(gRenderer, baseSurf);
	if (texture == NULL)
	{
		return false;
	}
	return true;
}

int BaseSprite::getWidth() { return scale_x * w; }
int BaseSprite::getHeight() { return scale_y * h; }

void BaseSprite::reset()
{
	clearMem();
	texture = NULL;
	baseSurf = NULL;
	w = 0;
	h = 0;
}

void BaseSprite::revert()
{
	w = baseSurf->w;
	h = baseSurf->h;

	SDL_DestroyTexture(texture);
	texture = SDL_CreateTextureFromSurface(gRenderer, baseSurf);
}

void BaseSprite::setWidth(int width) { w = width; }
void BaseSprite::setHeight(int height) { h = height; }
void BaseSprite::zoom(double rx, double ry)
{
	scale_x  = rx;
	scale_y = ry;
}

void BaseSprite::renderAt(int x, int y)
{
	SDL_Rect dest = { x - getWidth() / 2, y - getHeight() / 2, getWidth(), getHeight() };
	SDL_RenderCopy(gRenderer, texture, NULL, &dest);
}

void BaseSprite::clearMem()
{
	SDL_FreeSurface(baseSurf);
	SDL_DestroyTexture(texture);
}