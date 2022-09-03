#pragma once
#include <array>
#include <functional>
#include <memory>
#include <string>
#include "BaseSprite.h"
#include "BaseButtonAction.h"
#include "RenderCommands.h"


typedef std::array<int, 4> rect_t;

enum ButtonState
{
	BUTTON_DEFAULT,
	BUTTON_SELECTED,
	BUTTON_PRESSED,
	BUTTON_STATE_NUM
};

class MenuButton
{
	std::unique_ptr<BaseButtonAction> action;
	std::array<std::unique_ptr<BaseSprite>, BUTTON_STATE_NUM> sprites;
	std::function<void()> callback;
	rect_t rect;
	ButtonState state = BUTTON_DEFAULT;

	bool in_bounds(int x, int y);

public:
	MenuButton(rect_t rect, BaseButtonAction *action, std::function<void()> callback = []() {});
	MenuButton(MenuButton &&other) noexcept;
	MenuButton(MenuButton &&other, BaseButtonAction *action);

	BaseSprite &get_sprite() const;
	BaseSprite &get_sprite(ButtonState state) const;

	void set_sprite(ButtonState state, const BaseSprite &other);
	void set_sprite(ButtonState state, std::string path);
	void set_sprite(ButtonState state, std::string path, int mode);

	int center_x() const;
	int center_y() const;

	ButtonState get_state() const;
	bool select_at(int x, int y);
	bool press_at(int x, int y);
	void release_at(int x, int y);
};