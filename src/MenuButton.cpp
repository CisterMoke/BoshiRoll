#include "MenuButton.h"

MenuButton::MenuButton(rect_t rect, BaseButtonAction *action, std::function<void()> callback)
	: rect(rect), action(action), callback(callback) {};

MenuButton::MenuButton(MenuButton &&other) noexcept
{
	action.swap(other.action);
	for (int i = 0; i < BUTTON_STATE_NUM; i++)
	{
		sprites[i].swap(other.sprites[i]);
	}
	rect = other.rect;
	state = other.state;
	callback = other.callback;

	other.state = BUTTON_DEFAULT;
	other.rect = { 0, 0, 0, 0 };
	other.callback = []() {};
}

MenuButton::MenuButton(MenuButton &&other, BaseButtonAction *action)
	: MenuButton(std::move(other))
{
	this->action.reset(action);
}

BaseSprite &MenuButton::get_sprite() const
{
	return get_sprite(state);
}

BaseSprite &MenuButton::get_sprite(ButtonState state) const
{
	return *sprites[state];
}

void MenuButton::set_sprite(ButtonState state, const BaseSprite &other)
{
	sprites[state].reset(new BaseSprite(other));
	sprites[state]->set_width(rect[2] - rect[0]);
	sprites[state]->set_height(rect[3] - rect[1]);
}

void MenuButton::set_sprite(ButtonState state, std::string path)
{
	BaseSprite new_sprite = BaseSprite();
	new_sprite.load_from_file(path);
	set_sprite(state, new_sprite);
}

void MenuButton::set_sprite(ButtonState state, std::string path, int mode)
{
	BaseSprite new_sprite = BaseSprite();
	new_sprite.load_from_file(path, mode);
	set_sprite(state, new_sprite);
}

int MenuButton::center_x() const { return (rect[2] + rect[0]) / 2; }

int MenuButton::center_y() const { return (rect[3] + rect[1]) / 2; }

ButtonState MenuButton::get_state() const { return state; }

bool MenuButton::select_at(int x, int y)
{
	if (state != BUTTON_PRESSED && in_bounds(x, y))
	{
		state = BUTTON_SELECTED;
		return true;
	}

	state = BUTTON_DEFAULT;
	return false;
}

bool MenuButton::press_at(int x, int y)
{
	if (in_bounds(x, y))
	{
		state = BUTTON_PRESSED;
		return true;
	}
	return false;
}

void MenuButton::release_at(int x, int y)
{
	if (state == BUTTON_PRESSED)
	{ 
		action->execute();
		callback();
		in_bounds(x, y) ? state = BUTTON_DEFAULT : state = BUTTON_SELECTED;
	}
}

bool MenuButton::in_bounds(int x, int y)
{ 
	return rect[0] <= x && x <= rect[2] && rect[1] <= y && y <= rect[3];
}