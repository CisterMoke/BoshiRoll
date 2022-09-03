#include "MenuScreen.h"

bool MenuScreen::is_closed() { return closed; }

std::shared_ptr<MenuScreen> MenuScreen::get_next(int i)
{
	return 0 <= i && i < next_screens.size() ? next_screens[i] : nullptr;
}

void MenuScreen::add_next(std::shared_ptr<MenuScreen> next)
{
	next_screens.push_back(next);
}

void MenuScreen::add_button(MenuButton &&button)
{
	buttons.emplace_back(new MenuButton(std::move(button)));
}

void MenuScreen::add_close_button(MenuButton &&button)
{
	BaseButtonAction *close_action = new ButtonBoolToggle(closed);
	buttons.emplace_back( new MenuButton(std::move(button), close_action) );
}

void MenuScreen::add_text(const FontSprite &text, int x, int y)
{
	texts.emplace_back(text, x, y);
}

void MenuScreen::set_text(int idx, std::string text)
{
	if (idx < 0 || idx >= texts.size()) { return; }
	texts[idx].sprite.set_text(text);
}

void MenuScreen::reset()
{
	pressed.reset();
	selected.reset();
	closed = false;
}

void MenuScreen::handle_event(SDL_Event &event)
{

	switch (event.type)
	{

	case SDL_MOUSEMOTION:
		if (pressed != nullptr) { break; }
		for (auto &b : buttons)
		{
			if (b->select_at(event.motion.x, event.motion.y))
			{
				selected = b;
				break;
			}
		}
		break;

	case SDL_MOUSEBUTTONDOWN:
		for (auto &b : buttons)
		{
			if (b->press_at(event.button.x, event.button.y))
			{
				pressed = b;
				break;
			}
		}
		break;

	case SDL_MOUSEBUTTONUP:
		if (pressed != nullptr)
		{
			pressed->release_at(event.button.x, event.button.y);
			pressed = nullptr;
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