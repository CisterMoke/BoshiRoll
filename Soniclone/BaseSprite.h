#pragma once
#include <string>
#include <SDL.h>
#include <SDL_image.h>
#include "utils.h"
#include "Vec2.h"

class BaseSprite
{
	friend class Entity;
public:
	BaseSprite();
	~BaseSprite();

	bool loadFromFile(std::string path, int mode = 0x01);

	int getWidth();
	int getHeight();

	virtual void reset();
	void revert();
	void zoom(float rx, float ry);
	void flip(SDL_RendererFlip f);
	void toggleFlip(SDL_RendererFlip f);
	void rotate(float angle);
	void setWidth(int width);
	void setHeight(int height);
	void setTheta(float angle);
	void renderAt(int x, int y, SDL_Rect *clip = NULL);
	void renderAt(Vec2 const &pos, SDL_Rect *clip = NULL);

protected:
	void clearMem();
	void _render(SDL_Texture *tex, SDL_Rect *src = NULL, SDL_Rect *dst = NULL, float angle = 0, SDL_Point *center = NULL);

	int w = 0;
	int h = 0;
	float theta = 0.0;
	float scale_x = 1.0;
	float scale_y = 1.0;
	SDL_RendererFlip _flip = SDL_FLIP_NONE;

	SDL_Texture *texture = nullptr;
	SDL_Surface *baseSurf = nullptr;

};

