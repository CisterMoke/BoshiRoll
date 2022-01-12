#pragma once
#include <SDL_ttf.h>
#include <array>
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

	FontSprite(TTF_Font *font);

	void setText(std::string text);
	void setColor(SDL_Color color);
	void setFont(TTF_Font *font);

	void renderAtLU(int x, int y, Vec2 const &off = Vec2(0.0f, 0.0f), float phi = 0.0f, float zx = 1.0f, float zy = 1.0f, SDL_Rect *clip = NULL);
	void renderAt(int x, int y, Vec2 const &off = Vec2(0.0f, 0.0f), float phi = 0.0f, float zx = 1.0f, float zy = 1.0f, SDL_Rect *clip = NULL);


protected:
	std::string txt;
	std::vector<Vec2> offsets;
	std::vector<std::string> substrings;
	SDL_Color colr = { 0, 0, 0 };
	TTF_Font *_font;
	int tabsize = 50;
	int linespace;
	std::array<std::pair<SDL_Texture *, Vec2>*, 127> texture_cache;


private:
	//void createTexture();
	void createCharTexture(char c);
	bool loadFromFile(std::string path, int mode);
	void populate_cache();
	void clear_cache();
	void parseText(std::string txt);
};

