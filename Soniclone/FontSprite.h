#pragma once
#include <SDL_ttf.h>
#include <regex>
#include <string>
#include <utility>
#include <vector>
#include "globals.h"
#include "BaseSprite.h"
using namespace glob;
class FontSprite: public BaseSprite
{
public:
	int tabsize = 50;

	FontSprite();

	void setText(std::string text);
	void setColor(SDL_Color color);
	void setFont(TTF_Font *font);

	std::vector<std::pair<Vec2, std::string>> parseText(std::string txt);

protected:
	std::string txt = " ";
	SDL_Color colr = { 0, 0, 0 };
	TTF_Font *_font = nullptr;

private:
	void createTexture();
	bool loadFromFile(std::string path, int mode);

};

