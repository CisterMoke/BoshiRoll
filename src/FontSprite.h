#pragma once
#include <array>
#include <memory>
#include <regex>
#include <string>
#include <utility>
#include <vector>
#include "globals.h"
#include <SDL_ttf.h>
#include "BaseSprite.h"
#include "Mat22.h"
#include "FontTextureCache.h"


class FontSprite: public BaseSprite
{
public:

	FontSprite(TTF_Font *font);
	FontSprite(std::string text, std::string path, int size, SDL_Color color = {0, 0, 0});

	void set_text(std::string text);
	void set_color(SDL_Color color);
	void set_font(std::string path, int size);
	void set_tot_theta(float angle);
	void rotate_tot(float angle);
	void start_iter();
	bool next_iter();
	Vec2 get_char_pos(bool center = true);

	int get_tot_width() const;
	int get_tot_height() const;
	float get_tot_theta() const;
	float get_theta() const;
	float get_tot_degrees() const;
	float get_degrees() const;
	Mat22 get_cursor_transform() const;

protected:
	std::string txt{};
	std::vector<Vec2> offsets{};
	std::vector<std::string> substrings{};
	SDL_Color colr { 0, 0, 0 };
	int tabsize{ 50 };
	int linespace{};
	int curr_offset{ -1 }; int curr_char{ -1 };
	Vec2 cursor{ 0.0f, 0.0f };
	int w_tot, h_tot{ 0 };
	float tot_theta{ 0 };

	std::shared_ptr<TTF_Font> _font{};
	FontTextureCache texture_cache{};

private:
	void create_char_texture(char c);
	bool load_from_file(std::string path, int mode);
	void populate_cache();
	void clear_cache();
	void parse_text(std::string txt);
};

TTF_Font *load_font(std::string path, int size);