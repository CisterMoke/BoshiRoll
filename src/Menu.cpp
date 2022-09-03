#include "Menu.h"

Menu::Menu(std::shared_ptr<MenuScreen> start_screen)
{
	screens.push_front(start_screen);
};

bool Menu::is_closed() { return closed; }

void Menu::advance_screen()
{
	if (next_idx < 0 || next_idx >= screens.size()) { return; }

	screens.push_front(screens.front()->get_next(next_idx));
	next_idx = -1;
}

void Menu::close_screen()
{

	if (!screens.empty()) {
		screens.front()->reset();
		screens.pop_front();
	}
	if (screens.empty())
	{
		close();
	}
}

void Menu::close() { closed = true; }

void Menu::handle_input(SDL_Event &e)
{
	if (closed) { return; }
	screens.front()->handle_event(e);
	if (screens.front()->is_closed())
	{
		close_screen();
	}
	else if (next_idx >= 0) { advance_screen(); }
}

void Menu::push_render_cmds()
{
	for (BaseRenderCommand *cmd : screens.front()->render_cmds())
	{
		render_command_pool.push_back(cmd);
	}
}

std::shared_ptr<MenuScreen> MainMenu::initialize_screens()
{
	int CENTER_X = glob::SCREEN_WIDTH / 2; int CENTER_Y = glob::SCREEN_HEIGHT / 2;
	BaseSprite button_up = BaseSprite(glob::BUTTON_UP);
	BaseSprite button_over = BaseSprite(glob::BUTTON_OVER);
	BaseSprite button_down = BaseSprite(glob::BUTTON_DOWN);
	MenuButton start_button = MenuButton(
		{ CENTER_X - 50, CENTER_Y * 2/3 - 25, CENTER_X + 50, CENTER_Y * 2/3 + 25 },
		new ButtonBoolToggle(game_start));
	MenuButton options_open = MenuButton(
		{ CENTER_X - 50, CENTER_Y - 25, CENTER_X + 50, CENTER_Y + 25 },
		new ButtonSetValue(next_idx, 0));
	MenuButton quit_button = MenuButton(
	{  CENTER_X - 50, CENTER_Y * 4/3 - 25, CENTER_X + 50,CENTER_Y * 4/3 + 25 },
		nullptr);
	start_button.set_sprite(BUTTON_DEFAULT, button_up), start_button.set_sprite(BUTTON_SELECTED, button_over), start_button.set_sprite(BUTTON_PRESSED, button_down);
	options_open.set_sprite(BUTTON_DEFAULT, button_up), options_open.set_sprite(BUTTON_SELECTED, button_over), options_open.set_sprite(BUTTON_PRESSED, button_down);
	quit_button.set_sprite(BUTTON_DEFAULT, button_up), quit_button.set_sprite(BUTTON_SELECTED, button_over), quit_button.set_sprite(BUTTON_PRESSED, button_down);

	std::shared_ptr<MenuScreen> main_screen(new MenuScreen());
	main_screen->add_text(FontSprite("START", glob::COMIC_FONT_BOLD, 18, { 255, 255, 255 }),
		start_button.center_x(), start_button.center_y());
	main_screen->add_text(FontSprite("OPTIONS", glob::COMIC_FONT_BOLD, 18, { 255, 255, 255 }),
		options_open.center_x(), options_open.center_y());
	main_screen->add_text(FontSprite("QUIT", glob::COMIC_FONT_BOLD, 18, { 255, 255, 255 }),
		quit_button.center_x(), quit_button.center_y());
	main_screen->add_button(std::move(start_button)), main_screen->add_button(std::move(options_open)), main_screen->add_close_button(std::move(quit_button));
	
	std::shared_ptr<MenuScreen> options_screen(new MenuScreen());

	auto update_debug_text = [options_screen]()
	{
		options_screen->set_text(1, debug_mode_string());
	};

	MenuButton debug_button(
		{ CENTER_X - 50, CENTER_Y * 4/5 - 25, CENTER_X + 50, CENTER_Y * 4/5 + 25 },
		new ButtonScrollValue<DebugMode>(glob::DEBUG_MODE, { DEBUG_OFF, DEBUG_INFO, DEBUG_DRAW, DEBUG_ALL }),
		update_debug_text
	);
	MenuButton options_close(
		{ CENTER_X - 50, CENTER_Y * 6/5 - 25, CENTER_X + 50, CENTER_Y * 6/5 + 25 },
		nullptr
	);
	debug_button.set_sprite(BUTTON_DEFAULT, button_up), debug_button.set_sprite(BUTTON_SELECTED, button_over), debug_button.set_sprite(BUTTON_PRESSED, button_down);
	options_close.set_sprite(BUTTON_DEFAULT, button_up), options_close.set_sprite(BUTTON_SELECTED, button_over), options_close.set_sprite(BUTTON_PRESSED, button_down);

	options_screen->add_text(FontSprite("DEBUG_MODE:", glob::COMIC_FONT_BOLD, 18), debug_button.center_x() - 175, debug_button.center_y());
	options_screen->add_text(FontSprite(debug_mode_string(), glob::COMIC_FONT_BOLD, 18, {255, 255, 255}), debug_button.center_x(), debug_button.center_y());
	options_screen->add_text(FontSprite("BACK", glob::COMIC_FONT_BOLD, 18, {255, 255, 255}), options_close.center_x(), options_close.center_y());

	options_screen->add_button(std::move(debug_button));
	options_screen->add_close_button(std::move(options_close));

	main_screen->add_next(options_screen);
	
	return main_screen;

}

MainMenu::MainMenu()
	: Menu(initialize_screens()) {}

bool MainMenu::get_game_start() { return game_start; }

