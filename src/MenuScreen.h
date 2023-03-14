#pragma once
#include <vector>
#include "FontSprite.h"
#include "MenuButton.h"
#include "ButtonActions.h"
#include <SDL.h>

typedef std::function<void()> callback_t;

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
	std::vector<std::shared_ptr<MenuButton>> buttons{};
	std::vector<std::unique_ptr<BaseButtonAction>> actions{};
	std::vector<callback_t> callbacks{};
	std::vector<MenuText> texts{};
	std::vector<std::shared_ptr<MenuScreen>> next_screens{};

	int curr_button{ -1 };
	ButtonState curr_state{ BUTTON_DEFAULT };
	bool closed{ false };

	void update_current(int idx);
	void reset_current();

public:

	bool is_closed();

	MenuScreen &get_next(int i);

	MenuScreen &add_next();
	MenuScreen &add_next(std::shared_ptr<MenuScreen> next);
	MenuScreen &add_button(
		std::shared_ptr<MenuButton> button, BaseButtonAction *action = nullptr,
		callback_t callback = []() {}		
	);
	MenuScreen &add_close_button(
		std::shared_ptr<MenuButton> button, callback_t callback = []() {}
	);
	MenuScreen &add_text(const FontSprite &text, int x, int y);
	MenuScreen &set_text(int idx, std::string text);
	MenuScreen &reset();

	void handle_event(SDL_Event &event);

	std::vector<BaseRenderCommand*> render_cmds();
};