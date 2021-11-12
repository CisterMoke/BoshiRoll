#include "BaseSprite.h"
using glob::gRenderer;

BaseSprite::BaseSprite() = default;

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
	texture = nullptr;
	baseSurf = nullptr;
	w = 0;
	h = 0;
	theta = 0.0;
	scale_x = 1.0;
	scale_y = 1.0;
	_flip = SDL_FLIP_NONE;
}

void BaseSprite::revert()
{
	w = baseSurf->w;
	h = baseSurf->h;
	theta = 0.0;
	scale_x = 1.0;
	scale_y = 1.0;
	_flip = SDL_FLIP_NONE;

	SDL_DestroyTexture(texture);
	texture = SDL_CreateTextureFromSurface(gRenderer, baseSurf);
}

void BaseSprite::setWidth(int width) { w = width; }
void BaseSprite::setHeight(int height) { h = height; }
void BaseSprite::setTheta(float angle) { theta = angle; }
void BaseSprite::zoom(float rx, float ry)
{
	scale_x  = rx;
	scale_y = ry;
}

void BaseSprite::flip(SDL_RendererFlip f) { _flip = f; }
void BaseSprite::toggleFlip(SDL_RendererFlip f) { _flip =(SDL_RendererFlip)(_flip ^ f); }
void BaseSprite::rotate(float angle) { theta -= fmodf(angle, 360); }

void BaseSprite::renderAt(int x, int y, SDL_Rect *clip)
{
	SDL_Rect dest = { x - getWidth() / 2, y - getHeight() / 2, getWidth(), getHeight() };
	_render(texture, clip, &dest, theta, NULL);
}
void BaseSprite::renderAt(Vec2 const &pos, SDL_Rect *clip)
{
	renderAt(pos.x, pos.y, clip);
}

void BaseSprite::clearMem()
{
	SDL_FreeSurface(baseSurf);
	SDL_DestroyTexture(texture);
}


void BaseSprite::_render(SDL_Texture* tex, SDL_Rect *src, SDL_Rect *dst, float angle, SDL_Point *center)
{
	SDL_RenderCopyEx(gRenderer, tex, src, dst, angle, center, _flip);
}