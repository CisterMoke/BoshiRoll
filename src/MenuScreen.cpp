#include "MenuScreen.h"

void MenuScreen::update_current(int idx)
{
	curr_button = idx;
	curr_state = buttons[idx]->get_state();
}

void MenuScreen::reset_current()
{
	curr_button = -1;
	curr_state = BUTTON_DEFAULT;
}

bool MenuScreen::is_closed() { return closed; }

std::shared_ptr<MenuScreen> &MenuScreen::get_next(int i)
{
	return next_screens[i];
}

std::shared_ptr<MenuScreen> &MenuScreen::get_last_added()
{
	return next_screens.back();
}

MenuScreen &MenuScreen::add_next(std::shared_ptr<MenuScreen> next)
{
	next_screens.push_back(next);
	return *this;
}

MenuScreen &MenuScreen::add_button(
	std::shared_ptr<MenuButton> button, BaseButtonAction *action,
	callback_t callback
)
{
	buttons.push_back(button);
	callbacks.push_back(callback);
	actions.emplace_back(action);
	return *this;
}

MenuScreen &MenuScreen::add_close_button(
	std::shared_ptr<MenuButton> button, callback_t callback
)
{
	BaseButtonAction *close_action = new ButtonBoolToggle(closed);
	return add_button(button, close_action, callback);
}

MenuScreen &MenuScreen::add_text(const FontSprite &text, int x, int y)
{
	texts.emplace_back(text, x, y);
	return *this;
}

MenuScreen &MenuScreen::set_text(int idx, std::string text)
{
	if (idx < 0 || idx >= texts.size()) { return *this; }
	texts[idx].sprite.set_text(text);
	return *this;
}

MenuScreen &MenuScreen::reset()
{
	reset_current();
	closed = false;
	return *this;
}

void MenuScreen::handle_event(SDL_Event &event)
{

	switch (event.type)
	{

	case SDL_MOUSEMOTION:
		if (curr_state == BUTTON_PRESSED) { break; }
		for (int i = 0; i < buttons.size(); i++)
		{
			if (buttons[i]->select_at(event.motion.x, event.motion.y))
			{
				update_current(i);
				break;
			}
		}
		break;

	case SDL_MOUSEBUTTONDOWN:
		for (int i = 0; i < buttons.size(); i++)
		{
			if (buttons[i]->press_at(event.button.x, event.button.y))
			{
				update_current(i);
				break;
			}
		}
		break;

	case SDL_MOUSEBUTTONUP:
		if (curr_state == BUTTON_PRESSED)
		{
			buttons[curr_button]->release_at(event.button.x, event.button.y);
			actions[curr_button]->execute();
			callbacks[curr_button]();
			reset_current();
		}
		break;
	}
}

std::vector<BaseRenderCommand*> MenuScreen::render_cmds()
{
	std::vector<BaseRenderCommand*> cmds = {};
	for (auto b : buttons)
	{
		cmds.push_back(new RenderSpriteCommand<BaseSprite>(&b->get_sprite(), nullptr,
			Vec2(b->center_x(), b->center_y())));
	}
	for (MenuText &t : texts)
	{
		cmds.push_back(new RenderSpriteCommand<FontSprite>(&t.sprite, nullptr,
			Vec2(t.center_x, t.center_y)));
	}
	return cmds;
}