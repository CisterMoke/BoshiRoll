#include "FontSprite.h"

FontSprite::FontSprite() = default;

void FontSprite::setText(std::string text)
{
	txt = text;
	createTexture();
}

void FontSprite::setColor(SDL_Color color)
{
	colr = color;
	createTexture();
}

void FontSprite::setFont(TTF_Font *font)
{
	_font = font;
	createTexture();
}

std::vector<std::pair<Vec2, std::string>> FontSprite::parseText(std::string txt)
{
	std::vector<std::pair<Vec2, std::string>> pairs;

	std::regex newline("\n");
	std::sregex_token_iterator first{ txt.begin(), txt.end(), newline, -1 }, last;//the '-1' is what makes the regex split (-1 := what was not matched)
	std::vector<std::string> lines{ first, last };

	int x = 0;
	int y = 0;
	for (std::string line : lines)
	{
		std::regex tab("\t");
		std::sregex_token_iterator first{ line.begin(), line.end(), tab, -1 }, last;//the '-1' is what makes the regex split (-1 := what was not matched)
		std::vector<std::string> substrings{ first, last };
		int max_h = 0;

		for (std::string sub : substrings)
		{
			int last_h = 0;
			max_h = 0;

			if (sub != "")
			{
				pairs.push_back(std::pair(Vec2(x, y), sub));
			}
			x += tabsize;
			TTF_SizeText(_font, sub.c_str(), NULL, &last_h);
			max_h = fmaxf(max_h, last_h);
		}

		x = 0;
		y += max_h;
	}
	
	return pairs;
}

void FontSprite::createTexture()
{
	clearMem();
	if (_font == NULL) { return; }
	baseSurf = TTF_RenderText_Solid(_font, txt.c_str(), colr);
	texture = SDL_CreateTextureFromSurface(gRenderer, baseSurf);
	w = baseSurf->w;
	h = baseSurf->h;
}

bool FontSprite::loadFromFile(std::string path, int mode)
{
	return BaseSprite::loadFromFile(path, mode);
}
