#include "FontSprite.h"

FontSprite::FontSprite(TTF_Font *font)
	:_font(font, TTF_CloseFont)
{
	set_text(" ");
	TTF_SizeText(_font.get(), "A", nullptr, &linespace);
}

FontSprite::FontSprite(std::string text, std::string path, int size, SDL_Color color)
	: FontSprite(load_font(path, size))
{
	colr = color;
	set_text(text);
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

void FontSprite::set_font(std::string path, int size)
{
	TTF_Font *font = load_font(path, size);
	_font.reset(font, TTF_CloseFont);
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
	texture = texture_cache.get_texture(c);
	Vec2 dims = texture_cache.get_dim(c);
	px_w = dims.x; px_h = dims.y;
	cursor = Vec2(0.0f, 0.0f);
}

bool FontSprite::next_iter()
{
	if (curr_char < 0 || curr_offset < 0) { return false; }
	if (curr_offset == std::max((int) offsets.size() - 1, 0) && curr_char == std::max((int)substrings[curr_offset].size() - 1, 0)) { return false; }

	// Update to transform based on tot_theta.
	cursor.x += px_w;
	if (substrings[curr_offset].size() == 0) { curr_char = 0; }
	else { curr_char = ++curr_char % substrings[curr_offset].size(); }
	if (curr_char == 0)
	{
		curr_offset = ++curr_offset % offsets.size();
		cursor = offsets[curr_offset];
	}
	char c = substrings[curr_offset][curr_char];
	texture = texture_cache.get_texture(c);
	Vec2 dims = texture_cache.get_dim(c);
	px_w = dims.x; px_h = dims.y;
	return true;
}

Vec2 FontSprite::get_char_pos(bool center)
{
	Vec2 corr = center ? Vec2(px_w / 2, linespace - px_h / 2) : Vec2(0.0f, linespace - px_h);
	return ((cursor + corr - Vec2(w_tot/2, h_tot/2)) * get_cursor_transform() + Vec2(w_tot / 2, h_tot / 2)) * Vec2(scale_x, scale_y);
}

int FontSprite::get_tot_width() const
{
	return w_tot * scale_x;
}


int FontSprite::get_tot_height() const
{
	return h_tot * scale_y;
}

float FontSprite::get_tot_theta() const
{
	return tot_theta;
}

float FontSprite::get_theta() const
{
	return theta + tot_theta;
}

float FontSprite::get_tot_degrees() const
{
	return -fmodf(fmodf(tot_theta * 180.0f / M_PI, 360) + 360, 360);
}

float FontSprite::get_degrees() const
{
	return -fmodf(fmodf((theta + tot_theta) * 180.0f / M_PI, 360) + 360, 360);
}

Mat22 FontSprite::get_cursor_transform() const
{
	float phi = tot_theta;
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
	if (_font.get() == nullptr) { return; }
	if (c == 32)
	{
		base_surf.reset(TTF_RenderGlyph_Solid(_font.get(), c, colr), SDL_FreeSurface);
		texture.reset();
	}
	else
	{
		base_surf.reset(TTF_RenderGlyph_Solid(_font.get(), c, colr), SDL_FreeSurface);
		texture.reset(SDL_CreateTextureFromSurface(glob::g_renderer, base_surf.get()), SDL_DestroyTexture);
	}
	px_w = base_surf->w;
	px_h = base_surf->h;
}

bool FontSprite::load_from_file(std::string path, int mode)
{
	return BaseSprite::load_from_file(path, mode);
}

void FontSprite::populate_cache()
{
	int tot_w = 0;
	int tot_h = offsets[offsets.size() - 1].y + linespace;

	texture_cache.insert(0, std::shared_ptr<SDL_Texture>(), Vec2());

	for (int i = 0; i < substrings.size(); i++)
	{
		int linewidth = 0;
		std::string sub = substrings[i];
		for (char c : sub)
		{
			if (c < 1 && c > 127) { continue; }
			else if (!texture_cache.contains(c)) { 
				create_char_texture(c); 
				texture_cache.insert(c, texture, Vec2(px_w, px_h));
			}
			linewidth += texture_cache.get_dim(c).x;
		}
		if (linewidth + offsets[i].x > tot_w) { tot_w = linewidth + offsets[i].x; }
	}
	base_surf.reset();
	texture.reset();
	px_w = 0; px_h = 0;
	w_tot = tot_w;
	h_tot = tot_h;
}

void FontSprite::clear_cache()
{
	texture_cache.clear();
	base_surf.reset();
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