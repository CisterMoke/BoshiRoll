#pragma once
#include <deque>
#include <memory>
#include <vector>
#include <SDL.h>
#include "globals.h"
#include "utils.h"
#include "ButtonActions.h"
#include "DebugMode.h"
#include "MenuButton.h"
#include "MenuScreen.h"
#include "RenderCommands.h"

class Menu
{
private:
	std::shared_ptr<MenuScreen> root;
protected:
	std::deque<MenuScreen*> screens{};
	bool closed{ false };
	int next_idx{ -1 };

public:
	Menu(std::shared_ptr<MenuScreen> start_screen);
	bool is_closed();
	void advance_screen();
	void close_screen();
	void close();
	void handle_input(SDL_Event &e);
	void push_render_cmds();

	virtual ~Menu() = default;


};

class MainMenu : public Menu
{
	bool game_start=false;
	std::shared_ptr<MenuScreen> initialize_screens();
public:
	MainMenu();
	bool get_game_start();
};

class PauseMenu : public Menu
{
	bool game_resume{ false };
	std::shared_ptr<MenuScreen> initialize_screens();
public:
	PauseMenu();
	bool get_game_resume();
};