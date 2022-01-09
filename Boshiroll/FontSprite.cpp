#include "FontSprite.h"

FontSprite::FontSprite(TTF_Font *font)
	: _font(font)
{
	setText(" ");
	TTF_SizeText(_font, "A", nullptr, &linespace);
}

void FontSprite::setText(std::string text)
{
	txt = text;
	parseText(text);
	populate_cache();
}

void FontSprite::setColor(SDL_Color color)
{
	colr = color;
	clear_cache();
	populate_cache();
}

void FontSprite::setFont(TTF_Font *font)
{
	_font = font;
	clear_cache();
	linespace = TTF_SizeText(_font, "A", nullptr, &linespace);
	populate_cache();
}

void FontSprite::parseText(std::string txt)
{
	int tabs = 0;
	int newlines = 0;
	for (char c : txt)
	{
		if (c == '\n') { newlines++; }
		else if (c == '\t') { tabs++; }
	}

	offsets.resize(tabs + newlines + 1);
	substrings.resize(tabs + newlines + 1);

	if (tabs + newlines == 0)
	{
		offsets[0] = Vec2(0.0f, 0.0f);
		substrings[0] = txt;
	}
	else
	{
		int x = 0, y = 0, count = 0, begin = 0;
		for (int i = 0; i < txt.length(); i++)
		{
			if (txt[i] == '\t')
			{
				offsets[count] = Vec2(x, y);
				substrings[count] = txt.substr(begin, i - begin);
				count++;
				begin = i + 1;
				x += tabsize;
			}
			else if (txt[i] == '\n')
			{
				offsets[count] = Vec2(x, y);
				substrings[count] = txt.substr(begin, i - begin);
				count++;
				begin = i + 1;
				y += linespace;
				x = 0;
			}
		}
		offsets[tabs + newlines] = Vec2(x, y);
		substrings[tabs + newlines] = txt.substr(begin, txt.length());
	}
}

void FontSprite::createCharTexture(char c)
{
	if (_font == nullptr) { return; }
	SDL_FreeSurface(baseSurf);
	if (c == 32)
	{
		c = 45;
		baseSurf = TTF_RenderText_Solid(_font, &c, colr);
		texture = nullptr;
	}
	else
	{
		baseSurf = TTF_RenderText_Solid(_font, &c, colr);
		texture = SDL_CreateTextureFromSurface(gRenderer, baseSurf);
	}
	w = baseSurf->w;
	h = baseSurf->h;
}

bool FontSprite::loadFromFile(std::string path, int mode)
{
	return BaseSprite::loadFromFile(path, mode);
}

void FontSprite::populate_cache()
{
	int tot_w = offsets[offsets.size() - 1].x;
	int tot_h = offsets[offsets.size() - 1].y + linespace;

	for (int i = 0; i < substrings.size(); i++)
	{
		std::string sub = substrings[i];
		for (char c : sub)
		{
			if (0 < c < 128)
			{
				if (texture_cache[c] == nullptr)
				{
					createCharTexture(c);
					texture_cache[c] = new std::pair<SDL_Texture *, Vec2>{ texture, Vec2(w, h) };
				}
				if (i == substrings.size() - 1) { tot_w += texture_cache[c]->second.x; }
			}
		}
	}
	SDL_FreeSurface(baseSurf);
	baseSurf = nullptr;
	texture = nullptr;
	w = tot_w;
	h = tot_h;
}

void FontSprite::clear_cache()
{
	for (int i = 0; i < texture_cache.size(); i++)
	{
		if (texture_cache[i] != nullptr)
		{
			SDL_DestroyTexture(texture_cache[i]->first);
			delete texture_cache[i];
			texture_cache[i] = nullptr;
		}
	}
	texture = nullptr;
}

void FontSprite::renderAtLU(int x, int y, Vec2 const &off, float phi, float zx, float zy, SDL_Rect *clip)
{
	Vec2 cursor;
	int tot_w = w, tot_h = h;

	for (int i = 0; i < offsets.size(); i++)
	{
		cursor = Vec2(x, y) + offsets[i];
		for (char c : substrings[i])
		{
			texture = texture_cache[c]->first;
			Vec2 dims = texture_cache[c]->second;
			w = dims.x, h = dims.y;
			cursor.y += linespace - h;
			BaseSprite::renderAtLU(cursor, off, phi, zx, zy, clip);
			cursor.y = y + offsets[i].y;
			cursor.x += w;
		}
	}
	w = tot_w, h = tot_h;
}

void FontSprite::renderAt(int x, int y, Vec2 const &off, float phi, float zx, float zy, SDL_Rect *clip)
{
	Vec2 cursor;
	int tot_w = w, tot_h = h;
	x -= getWidth() / 2;
	y -= getHeight() / 2;

	for (int i = 0; i < offsets.size(); i++)
	{
		cursor = Vec2(x, y) + offsets[i];
		for (char c : substrings[i])
		{
			texture = texture_cache[c]->first;
			Vec2 dims = texture_cache[c]->second;
			w = dims.x, h = dims.y;
			cursor.y += linespace - h / 2;
			cursor.x += w / 2;
			BaseSprite::renderAt(cursor, off, phi, zx, zy, clip);
			cursor.y = y + offsets[i].y;
			cursor.x += w - w/2;
		}
	}
	w = tot_w, h = tot_h;
}
