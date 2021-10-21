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
	void setWidth(int width);
	void setHeight(int height);
	void renderAt(int x, int y);

protected:
	void clearMem();

	int w;
	int h;
	double scale_x;
	double scale_y;

	SDL_Texture *texture;
	SDL_Surface *baseSurf;

};

