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