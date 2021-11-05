#pragma once
#include <SDL_ttf.h>
#include <string>
#include "globals.h"
#include "BaseSprite.h"
using namespace glob;
class FontSprite: public BaseSprite
{
public:
	FontSprite();

	void setText(std::string text);
	void setColor(SDL_Color color);
	void setFont(TTF_Font *font);

protected:
	std::string txt = " ";
	SDL_Color colr = { 0, 0, 0 };
	TTF_Font *_font = nullptr;

private:
	void createTexture();
	bool loadFromFile(std::string path, int mode);
};

