#include "SpinSprite.h"
using glob::gRenderer;

SpinSprite::SpinSprite()
{
	theta = 0;
};

void SpinSprite::reset()
{
	BaseSprite::reset();
	theta = 0;
}

void SpinSprite::revert() 
{
	BaseSprite::revert();
	theta = 0;
}
void SpinSprite:: rotate(double angle, int smoothing)
{
	theta -= fmod(angle, 360);
	SDL_Surface *baseCopy = SDL_ConvertSurface(baseSurf, baseSurf->format, 0); //Somehow using baseSurf directly does not preserve the colorkey
	SDL_Surface *rotated = rotozoomSurface(baseCopy, theta, 1, smoothing);

	w = rotated->w;
	h = rotated->h;

	SDL_DestroyTexture(texture);
	texture = SDL_CreateTextureFromSurface(gRenderer, rotated);
	SDL_FreeSurface(baseCopy);
	SDL_FreeSurface(rotated);
}

void SpinSprite::renderAt(int x, int y, double zoom)
{
	SDL_Rect dest = { x - getWidth() / 2, y - getHeight() / 2, getWidth(), getHeight() };
	SDL_RenderCopy(gRenderer, texture, NULL, &dest);
}