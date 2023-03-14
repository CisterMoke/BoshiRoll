#include "Menu.h"

Menu::Menu(std::shared_ptr<MenuScreen> start_screen)
	:root(start_screen)
{
	screens.push_front(root.get());
};

bool Menu::is_closed() { return closed; }

void Menu::advance_screen()
{
	if (next_idx < 0 || next_idx >= screens.size()) { return; }

	screens.push_front(&screens.front()->get_next(next_idx));
	next_idx = -1;
}

void Menu::close_screen()
{

	if (!screens.empty()) {
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

	std::shared_ptr<MenuButton> start_button = std::make_shared<MenuButton>(
		MenuButton({ CENTER_X - 50, CENTER_Y * 2 / 3 - 25, CENTER_X + 50, CENTER_Y * 2 / 3 + 25 })
		);
	std::shared_ptr<MenuButton> options_open = std::make_shared<MenuButton>(
		MenuButton({ CENTER_X - 50, CENTER_Y - 25, CENTER_X + 50, CENTER_Y + 25 })
		);
	std::shared_ptr<MenuButton> quit_button = std::make_shared<MenuButton>(
		MenuButton({ CENTER_X - 50, CENTER_Y * 4 / 3 - 25, CENTER_X + 50,CENTER_Y * 4 / 3 + 25 })
		);

	start_button->set_sprite(BUTTON_DEFAULT, glob::BUTTON_UP).set_sprite(BUTTON_SELECTED, glob::BUTTON_OVER).set_sprite(BUTTON_PRESSED, glob::BUTTON_DOWN);
	options_open->copy_sprites(*start_button);
	quit_button->copy_sprites(*start_button);

	std::shared_ptr<MenuScreen> main_screen = std::make_shared<MenuScreen>();

	main_screen->add_text(FontSprite("START", glob::COMIC_FONT_BOLD, 18, { 255, 255, 255 }),
		start_button->center_x(), start_button->center_y());
	main_screen->add_text(FontSprite("OPTIONS", glob::COMIC_FONT_BOLD, 18, { 255, 255, 255 }),
		options_open->center_x(), options_open->center_y());
	main_screen->add_text(FontSprite("QUIT", glob::COMIC_FONT_BOLD, 18, { 255, 255, 255 }),
		quit_button->center_x(), quit_button->center_y());

	main_screen->add_button(start_button, new ButtonBoolToggle(game_start));
	main_screen->add_button(options_open, new ButtonSetValue(next_idx, 0));
	main_screen->add_close_button(quit_button);

	MenuScreen &options_screen = main_screen->add_next();

	auto update_debug_text = [&options_screen]()
	{
		options_screen.set_text(1, debug_mode_string());
	};

	std::shared_ptr<MenuButton> debug_button = std::make_shared<MenuButton>(
		MenuButton({ CENTER_X - 50, CENTER_Y * 4 / 5 - 25, CENTER_X + 50, CENTER_Y * 4 / 5 + 25 })
		);
	std::shared_ptr<MenuButton> options_close = std::make_shared<MenuButton>(
		MenuButton({ CENTER_X - 50, CENTER_Y * 6 / 5 - 25, CENTER_X + 50, CENTER_Y * 6 / 5 + 25 })
		);

	debug_button->copy_sprites(*start_button);
	options_close->copy_sprites(*start_button);

	options_screen.add_text(FontSprite("DEBUG_MODE:", glob::COMIC_FONT_BOLD, 18), debug_button->center_x() - 175, debug_button->center_y());
	options_screen.add_text(FontSprite(debug_mode_string(), glob::COMIC_FONT_BOLD, 18, {255, 255, 255}), debug_button->center_x(), debug_button->center_y());
	options_screen.add_text(FontSprite("BACK", glob::COMIC_FONT_BOLD, 18, {255, 255, 255}), options_close->center_x(), options_close->center_y());

	options_screen.add_button(
		debug_button,
		new ButtonScrollValue<DebugMode>(glob::DEBUG_MODE, { DEBUG_OFF, DEBUG_INFO, DEBUG_DRAW, DEBUG_ALL }),
		update_debug_text);
	options_screen.add_close_button(options_close);
	
	return main_screen;
}

MainMenu::MainMenu()
	: Menu(initialize_screens()) {}

bool MainMenu::get_game_start() { return game_start; }

std::shared_ptr<MenuScreen> PauseMenu::initialize_screens()
{
	int CENTER_X = glob::SCREEN_WIDTH / 2; int CENTER_Y = glob::SCREEN_HEIGHT / 2;
	BaseSprite button_up = BaseSprite(glob::BUTTON_UP);
	BaseSprite button_over = BaseSprite(glob::BUTTON_OVER);
	BaseSprite button_down = BaseSprite(glob::BUTTON_DOWN);

	std::shared_ptr<MenuButton> resume_button = std::make_shared<MenuButton>(
		MenuButton({ CENTER_X - 50, CENTER_Y * 2 / 3 - 25, CENTER_X + 50, CENTER_Y * 2 / 3 + 25 })
		);
	std::shared_ptr<MenuButton> options_open = std::make_shared<MenuButton>(
		MenuButton({ CENTER_X - 50, CENTER_Y - 25, CENTER_X + 50, CENTER_Y + 25 })
		);
	std::shared_ptr<MenuButton> quit_button = std::make_shared<MenuButton>(
		MenuButton({ CENTER_X - 50, CENTER_Y * 4 / 3 - 25, CENTER_X + 50,CENTER_Y * 4 / 3 + 25 })
		);

	resume_button->set_sprite(BUTTON_DEFAULT, glob::BUTTON_UP).set_sprite(BUTTON_SELECTED, glob::BUTTON_OVER).set_sprite(BUTTON_PRESSED, glob::BUTTON_DOWN);
	options_open->copy_sprites(*resume_button);
	quit_button->copy_sprites(*resume_button);
	
	std::shared_ptr<MenuScreen> pause_screen = std::make_shared<MenuScreen>();
	
	pause_screen->add_text(FontSprite("RESUME", glob::COMIC_FONT_BOLD, 18, { 255, 255, 255 }),
		resume_button->center_x(), resume_button->center_y());
	pause_screen->add_text(FontSprite("OPTIONS", glob::COMIC_FONT_BOLD, 18, { 255, 255, 255 }),
		options_open->center_x(), options_open->center_y());
	pause_screen->add_text(FontSprite("QUIT", glob::COMIC_FONT_BOLD, 18, { 255, 255, 255 }),
		quit_button->center_x(), quit_button->center_y());

	pause_screen->add_button(resume_button, new ButtonBoolToggle(game_resume));
	pause_screen->add_button(options_open, new ButtonSetValue(next_idx, 0));
	pause_screen->add_close_button(quit_button);

	MenuScreen &options_screen = pause_screen->add_next();

	auto update_debug_text = [&options_screen]()
	{
		options_screen.set_text(1, debug_mode_string());
	};

	std::shared_ptr<MenuButton> debug_button = std::make_shared<MenuButton>(
		MenuButton({ CENTER_X - 50, CENTER_Y * 4 / 5 - 25, CENTER_X + 50, CENTER_Y * 4 / 5 + 25 })
		);
	std::shared_ptr<MenuButton> options_close = std::make_shared<MenuButton>(
		MenuButton({ CENTER_X - 50, CENTER_Y * 6 / 5 - 25, CENTER_X + 50, CENTER_Y * 6 / 5 + 25 })
		);
	debug_button->copy_sprites(*resume_button);
	options_close->copy_sprites(*resume_button);

	options_screen.add_text(FontSprite("DEBUG_MODE:", glob::COMIC_FONT_BOLD, 18), debug_button->center_x() - 175, debug_button->center_y());
	options_screen.add_text(FontSprite(debug_mode_string(), glob::COMIC_FONT_BOLD, 18, { 255, 255, 255 }), debug_button->center_x(), debug_button->center_y());
	options_screen.add_text(FontSprite("BACK", glob::COMIC_FONT_BOLD, 18, { 255, 255, 255 }), options_close->center_x(), options_close->center_y());

	options_screen.add_button(
		debug_button,
		new ButtonScrollValue<DebugMode>(glob::DEBUG_MODE, { DEBUG_OFF, DEBUG_INFO, DEBUG_DRAW, DEBUG_ALL }),
		update_debug_text
		);
	options_screen.add_close_button(options_close);

	return pause_screen;
}

PauseMenu::PauseMenu()
	: Menu(initialize_screens()) {}

bool PauseMenu::get_game_resume() { return game_resume; }