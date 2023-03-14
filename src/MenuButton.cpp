#include "MenuButton.h"

MenuButton::MenuButton(rect_t rect)
	: rect(rect) {};

BaseSprite &MenuButton::get_sprite() const
{
	return get_sprite(state);
}

BaseSprite &MenuButton::get_sprite(ButtonState state) const
{
	return *sprites[state];
}

MenuButton &MenuButton::set_sprite(ButtonState state, std::shared_ptr<BaseSprite> sprite)
{
	sprites[state] = sprite;
	sprites[state]->set_width(rect[2] - rect[0]);
	sprites[state]->set_height(rect[3] - rect[1]);
	return *this;
}

MenuButton &MenuButton::set_sprite(ButtonState state, std::string path)
{
	set_sprite(state, std::make_shared<BaseSprite>(path));
	return *this;
}

MenuButton &MenuButton::set_sprite(ButtonState state, std::string path, int mode)
{
	set_sprite(state, std::make_shared<BaseSprite>(path, mode));
	return *this;
}

MenuButton &MenuButton::copy_sprites(const MenuButton &other)
{
	sprites = other.sprites;
	return *this;
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
		in_bounds(x, y) ? state = BUTTON_DEFAULT : state = BUTTON_SELECTED;
	}
}

bool MenuButton::in_bounds(int x, int y)
{ 
	return rect[0] <= x && x <= rect[2] && rect[1] <= y && y <= rect[3];
}