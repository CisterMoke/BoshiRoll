#pragma once
#include <string>
#include <SDL.h>
#include <SDL_image.h>
#include "utils.h"

class BaseSprite
{
public:
	BaseSprite();
	~BaseSprite();

	bool loadFromFile(std::string path, int mode = 0x01);

	int getWidth();
	int getHeight();

	virtual void reset();
	void revert();
	void zoom(double rx, double ry);
	void flip(SDL_RendererFlip f);
	void rotate(double angle);
	void setWidth(int width);
	void setHeight(int height);
	void renderAt(int x, int y, SDL_Rect *clip = NULL);

protected:
	void clearMem();
	void _render(SDL_Texture *tex, SDL_Rect *src = NULL, SDL_Rect *dst = NULL, double angle = 0, SDL_Point *center = NULL);

	int w = 0;
	int h = 0;
	double theta = 0;
	double scale_x = 1;
	double scale_y = 1;
	SDL_RendererFlip _flip = SDL_FLIP_NONE;

	SDL_Texture *texture = nullptr;
	SDL_Surface *baseSurf = nullptr;

};

