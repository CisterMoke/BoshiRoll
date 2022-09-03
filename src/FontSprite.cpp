#include "FontSprite.h"

FontSprite::FontSprite(TTF_Font *font)
	: _font(font, TTF_CloseFont)
{
	texture_cache.fill(nullptr);
	set_text(" ");
	TTF_SizeText(_font.get(), "A", nullptr, &linespace);
}

FontSprite::FontSprite(std::string text, std::string path, int size, SDL_Color color)
	: FontSprite(load_font(path, size))
{
	colr = color;
	set_text(text);
}

FontSprite::FontSprite(const FontSprite &other): BaseSprite(other)
{
	txt = other.txt;
	offsets = other.offsets;
	substrings = other.substrings;
	colr = other.colr;
	_font = other._font;
	tabsize = other.tabsize;
	linespace = other.linespace;
	texture_cache = other.texture_cache;
	curr_offset = other.curr_offset; curr_char = other.curr_char;
	cursor = other.cursor;
	w_tot = other.w_tot, h_tot = other.h_tot;
}

void FontSprite::set_text(std::string text)
{
	txt = text;
	parse_text(text);
	populate_cache();
}

void FontSprite::set_color(SDL_Color color)
{
	colr = color;
	clear_cache();
	populate_cache();
}

void FontSprite::set_font(TTF_Font *font)
{
	_font = std::shared_ptr<TTF_Font>(font);
	clear_cache();
	linespace = TTF_SizeText(_font.get(), "A", nullptr, &linespace);
	populate_cache();
}

void FontSprite::set_tot_theta(float angle)
{
	tot_theta = angle;
}

void FontSprite::rotate_tot(float angle)
{
	tot_theta += angle;
}

void FontSprite::start_iter()
{
	curr_offset = 0; curr_char = 0;
	char c = substrings[curr_offset][curr_char];
	texture = texture_cache[c]->first;
	Vec2 dims = texture_cache[c]->second;
	w = dims.x; h = dims.y;
	cursor = Vec2(0.0f, 0.0f);
}

bool FontSprite::next_iter()
{
	if (curr_char < 0 || curr_offset < 0) { return false; }
	if (curr_offset == std::max((int) offsets.size() - 1, 0) && curr_char == std::max((int)substrings[curr_offset].size() - 1, 0)) { return false; }

	// Update to transform based on tot_theta.
	cursor.x += w;
	if (substrings[curr_offset].size() == 0) { curr_char = 0; }
	else { curr_char = ++curr_char % substrings[curr_offset].size(); }
	if (curr_char == 0)
	{
		curr_offset = ++curr_offset % offsets.size();
		cursor = offsets[curr_offset];
	}
	char c = substrings[curr_offset][curr_char];
	texture = texture_cache[c]->first;
	Vec2 dims = texture_cache[c]->second;
	w = dims.x; h = dims.y;
	return true;
}

Vec2 FontSprite::get_char_pos(bool center)
{
	Vec2 corr = center ? Vec2(w / 2, linespace - h / 2) : Vec2(0.0f, linespace - h);
	return ((cursor + corr - Vec2(w_tot/2, h_tot/2)) * get_cursor_transform() + Vec2(w_tot / 2, h_tot / 2)) * Vec2(scale_x, scale_y);
}

int FontSprite::get_tot_width()
{
	return w_tot * scale_x;
}


int FontSprite::get_tot_height()
{
	return h_tot * scale_y;
}

float FontSprite::get_tot_theta()
{
	return tot_theta;
}

float FontSprite::get_theta()
{
	return theta + tot_theta;
}

Mat22 FontSprite::get_cursor_transform()
{
	float phi = -tot_theta * M_PI / 180.0f;
	return rotMat(phi);
}

void FontSprite::parse_text(std::string txt)
{
	curr_offset = 0; curr_char = 0;
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

void FontSprite::create_char_texture(char c)
{
	if (_font == nullptr) { return; }
	if (c == 32)
	{
		c = 45;
		base_surf.reset(TTF_RenderText_Solid(_font.get(), &c, colr), SDL_FreeSurface);
		texture.reset();
	}
	else
	{
		base_surf.reset(TTF_RenderText_Solid(_font.get(), &c, colr), SDL_FreeSurface);
		texture.reset(SDL_CreateTextureFromSurface(glob::g_renderer, base_surf.get()), SDL_DestroyTexture);
	}
	w = base_surf->w;
	h = base_surf->h;
}

bool FontSprite::load_from_file(std::string path, int mode)
{
	return BaseSprite::load_from_file(path, mode);
}

void FontSprite::populate_cache()
{
	int tot_w = 0;
	int tot_h = offsets[offsets.size() - 1].y + linespace;

	texture_cache[0] = new std::pair<std::shared_ptr<SDL_Texture>, Vec2>{ std::shared_ptr<SDL_Texture>(), Vec2() };

	for (int i = 0; i < substrings.size(); i++)
	{
		int linewidth = 0;
		std::string sub = substrings[i];
		for (char c : sub)
		{
			if (0 < c && c < 128)
			{
				if (texture_cache[c] == nullptr)
				{
					create_char_texture(c);
					texture_cache[c] = new std::pair<std::shared_ptr<SDL_Texture>, Vec2>{ texture, Vec2(w, h) };
				}
				linewidth += texture_cache[c]->second.x;
			}
		}
		if (linewidth + offsets[i].x > tot_w) { tot_w = linewidth + offsets[i].x; }
	}
	base_surf.reset();
	texture.reset();
	w_tot = tot_w;
	h_tot = tot_h;
}

void FontSprite::clear_cache()
{
	for (int i = 0; i < texture_cache.size(); i++)
	{
		if (texture_cache[i] != nullptr)
		{
			texture_cache[i]->first.reset();
		}
	}
	texture.reset();
}


TTF_Font *load_font(std::string path, int size)
{
	TTF_Font *font = TTF_OpenFont(path.c_str(), size);
	if (font == NULL)
	{
		std::cout << "Unable to load font " << path << std::endl;
		return NULL;
	}
	return font;
}