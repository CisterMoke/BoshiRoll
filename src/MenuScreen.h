#pragma once
#include <SDL.h>
#include <vector>
#include "FontSprite.h"
#include "MenuButton.h"
#include "ButtonActions.h"

struct MenuText
{
	FontSprite sprite;
	int center_x;
	int center_y;

	MenuText(const FontSprite &sprite, int x, int y)
		: sprite(sprite), center_x(x), center_y(y) {};
};


class MenuScreen
{
	std::vector<std::shared_ptr<MenuButton>> buttons = {};
	std::vector<MenuText> texts = {};
	std::vector<std::shared_ptr<MenuScreen>> next_screens = {};

	std::shared_ptr<MenuButton> pressed = nullptr;
	std::shared_ptr<MenuButton> selected = nullptr;
	bool closed = false;

public:

	bool is_closed();

	std::shared_ptr<MenuScreen> get_next(int i);

	void add_next(std::shared_ptr<MenuScreen> next);
	void add_button(MenuButton &&button);
	void add_close_button(MenuButton &&button);
	void add_text(const FontSprite &text, int x, int y);
	void set_text(int idx, std::string text);
	void reset();

	void handle_event(SDL_Event &event);

	std::vector<BaseRenderCommand*> render_cmds();
};