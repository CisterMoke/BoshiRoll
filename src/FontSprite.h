#pragma once
#include <SDL_ttf.h>
#include <array>
#include <memory>
#include <regex>
#include <string>
#include <utility>
#include <vector>
#include "BaseSprite.h"
#include "Mat22.h"
using namespace glob;
class FontSprite: public BaseSprite
{
public:

	FontSprite(TTF_Font *font);
	FontSprite(const FontSprite &other);

	void set_text(std::string text);
	void set_color(SDL_Color color);
	void set_font(TTF_Font *font);
	void set_tot_theta(float angle);
	void rotate_tot(float angle);
	void start_iter();
	bool next_iter();
	Vec2 get_char_pos(bool center = true);

	int get_tot_width();
	int get_tot_height();
	float get_tot_theta();
	float get_theta();
	Mat22 get_cursor_transform();

protected:
	std::string txt;
	std::vector<Vec2> offsets;
	std::vector<std::string> substrings;
	SDL_Color colr = { 0, 0, 0 };
	std::shared_ptr<TTF_Font> _font;
	int tabsize = 50;
	int linespace;
	std::array<std::pair<std::shared_ptr<SDL_Texture>, Vec2>*, 127> texture_cache;
	int curr_offset = -1; int curr_char = -1;
	Vec2 cursor = Vec2(0.0f, 0.0f);
	int w_tot, h_tot = 0;
	float tot_theta = 0;


private:
	void create_char_texture(char c);
	bool load_from_file(std::string path, int mode);
	void populate_cache();
	void clear_cache();
	void parse_text(std::string txt);
};

