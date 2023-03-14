#define SDL_MAIN_HANDLED
#include <memory>
#include <iostream>
#include <sstream>
#include <SDL.h>
#include <SDL_image.h>
#include <SDL_ttf.h>
#include "globals.h"
#include "utils.h"
#include "debug.h"
#include "FontSprite.h"
#include "Game.h"
#include "Camera.h"
#include "Renderer.h"
#include "Menu.h"

//Main gameloop functions
bool init();
bool load_media();
void close();

//Main surfaces and textures
SDL_Surface *g_title_img{};
SDL_Texture *g_title_text{};

SDL_Surface *g_surf{};
SDL_Texture *g_texture{};

Renderer renderer;
FontSprite *title_font;
FontSprite *debug_font;

std::unique_ptr<MainMenu> main_menu{};
std::unique_ptr<PauseMenu> pause_menu{};

std::unique_ptr<Game> game{};
Camera cam = Camera();
SDL_Color collor = { 180, 0, 180 };

std::stringstream title_text;
int boshi_split = 0;

bool init()
{
	// Init root
	if (SDL_Init(SDL_INIT_EVERYTHING) < 0)
	{
		std::cout << "SDL could not initialize! SDL_Error: " << SDL_GetError() << "\n";
		return false;
	}

	//Create window
	glob::g_window = SDL_CreateWindow("BoshiRoll", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, glob::SCREEN_WIDTH, glob::SCREEN_HEIGHT, SDL_WINDOW_OPENGL);
	if (glob::g_window == NULL)
	{
		std::cout << "Window could not be created!" << SDL_GetError() << "\n";
		return false;
	}
	//Create renderer for window
	glob::g_renderer = SDL_CreateRenderer(glob::g_window, -1, SDL_RENDERER_ACCELERATED);
	if (glob::g_renderer == NULL)
	{
		printf("Renderer could not be created! SDL Error: %s\n", SDL_GetError());
		return false;
	}
	//Initialize renderer color
	SDL_SetRenderDrawColor(glob::g_renderer, 0xFF, 0xFF, 0xFF, 0xFF);

	// Init png loading
	int img_flags = IMG_INIT_PNG;
	if (!(IMG_Init(img_flags) & img_flags))
	{
		std::cout << "SDL_image could not initialize! SDL_image Error: " << IMG_GetError() << "\n";
		return false;
	}
	// Init TTF font loading
	if (TTF_Init() == -1)
	{
		printf("TTF_Init: %s\n", TTF_GetError());
		return false;
	}
	renderer.renderer = glob::g_renderer;
	glob::g_screen = SDL_GetWindowSurface(glob::g_window);
	SDL_FillRect(glob::g_screen, NULL, SDL_MapRGB(glob::g_screen->format, 0xFF, 0xFF, 0xFF));
	SDL_UpdateWindowSurface(glob::g_window);
	return true;
}

bool load_media()
{
	g_title_img = load_surface(glob::TITLE_IMG);
	if (g_title_img == NULL)
	{
		return false;
	}

	g_title_text = SDL_CreateTextureFromSurface(glob::g_renderer, g_title_img);
	SDL_SetRenderDrawColor(glob::g_renderer, 255, 255, 255, 0 );

	title_font = new FontSprite(load_font(glob::COMIC_FONT_BOLD, 54));
	title_font->set_color({ 22, 26, 255 });

	debug_font = new FontSprite(load_font(glob::COMIC_FONT_BOLD, 16));

	return true;
}

void load_game()
{
	game.reset(new Game(cam));
}

void close()
{
	SDL_DestroyTexture(g_texture);
	g_texture = nullptr;
	SDL_FreeSurface(g_title_img);
	g_title_img = nullptr;
	SDL_FreeSurface(g_surf);
	g_surf = nullptr;

	SDL_DestroyRenderer(glob::g_renderer);
	SDL_DestroyWindow(glob::g_window);

	//Quit SDL subsystems
	IMG_Quit();
	TTF_Quit();
	SDL_Quit();
}

void tic(unsigned int *t) { *t = SDL_GetTicks(); }

unsigned int toc(unsigned int *t) { return SDL_GetTicks() - *t; }

void do_action(SDL_Event &event)
{
	if (event.type == SDL_KEYDOWN)
	{
		switch (event.key.keysym.sym)
		{

		case SDLK_ESCAPE:
			game->pause();
			break;

		case SDLK_u:
			cam.theta -= 5;
			break;

		case SDLK_o:
			cam.theta += 5;
			break;

		case SDLK_p:
			cam.theta = 0;
			cam.rx = 1.0f;
			cam.ry = 1.0f;
			break;

		case SDLK_r:
			game->player.pos = Vec2(glob::SCREEN_WIDTH / 2, glob::SCREEN_HEIGHT / 2);
			game->player.stop();
			break;
		case SDLK_F3:
			if (glob::DEBUG_MODE == DEBUG_OFF) { glob::DEBUG_MODE = DEBUG_ALL; }
			else { glob::DEBUG_MODE = DEBUG_OFF; }
			break;
		}
	}
	else if (event.type == SDL_MOUSEWHEEL)
	{
		if (event.wheel.y > 0)
		{
			cam.rx *= 1.1;
			cam.ry *= 1.1;
		}
		else if (event.wheel.y < 0)
		{
			cam.rx /= 1.1;
			cam.ry /= 1.1;
		}

	}
	game->player.do_action(event);
}

float title_angle = 0.0f;
void render()
{
	if (main_menu.get() != nullptr)
	{
		title_font->rotate_tot(1.5f);
		SDL_RenderClear(glob::g_renderer);
		title_text.str("");
		title_text << glob::TITLE_TEXT.substr(boshi_split) << glob::TITLE_TEXT.substr(0, boshi_split);
		title_font->set_text(title_text.str());
		render_command_pool.push_back(new RenderSpriteCommand<FontSprite>(title_font, NULL, Vec2(glob::SCREEN_WIDTH/2, glob::SCREEN_HEIGHT/2)));
		main_menu->push_render_cmds();
		
	}

	else if (game.get() != nullptr)
	{
		Vec2 orig = cam.get_origin();
		Vec2 off = Vec2(glob::SCREEN_WIDTH / 2, glob::SCREEN_HEIGHT / 2);
		game->push_render_commands();

		if ((glob::DEBUG_MODE & DEBUG_DRAW) == DEBUG_DRAW)
		{
			for (auto &collider : game->curr_level.get_colliders())
			{
				render_command_pool.push_back(collider->create_cmd(&cam));
			}

			for (auto &enemy : game->curr_level.get_enemies())
			{
				render_command_pool.push_back(enemy.collider->create_cmd(&cam));
			}

			render_command_pool.push_back(game->player.collider->create_cmd(&cam));

			if (game->player.get_tongue().get_state() != TongueState::IDLE)
			{
				render_command_pool.push_back(game->player.get_tongue().parts[0]->collider->create_cmd(&cam));
			}
		}

		if ((glob::DEBUG_MODE & DEBUG_INFO) == DEBUG_INFO) {
			push_entity_info_render_command(&game->player, debug_font);
		}
		
		if (pause_menu.get() != nullptr)
		{
			pause_menu->push_render_cmds();
		}
	}

	renderer.render_all();

	SDL_SetRenderDrawColor(glob::g_renderer, 255, 255, 255, 255);
	SDL_RenderPresent(glob::g_renderer);

}

int main(int argc, char *argv[])
{
	bool quit = false;
	SDL_Event e;
	unsigned int *render_timer = new unsigned int(0);
	unsigned int *game_timer = new unsigned int(0);
	unsigned int *boshi_timer = new unsigned int(0);
	tic(render_timer);
	tic(game_timer);
	tic(boshi_timer);

	SDL_SetMainReady();
	init();
	if (!load_media())
	{
		std::cout << "Failed to load media!\n";
		return 0;
	}
	main_menu.reset(new MainMenu());
	while (!quit)
	{
		while (SDL_PollEvent(&e) != 0)
		{
			if (e.type == SDL_QUIT)
			{
				quit = true;
				break;
			}
			else if (main_menu.get() != nullptr)
			{
				main_menu->handle_input(e);
				if (main_menu->get_game_start())
				{
					load_game();
					main_menu.reset();
				}
				else if (main_menu->is_closed())
				{
					main_menu.reset();
					quit = true;
					break;
				}
			}

			else if (game->is_paused())
			{
				if (pause_menu.get() == nullptr) {
					pause_menu.reset(new PauseMenu());
				}
				pause_menu->handle_input(e);
				if (pause_menu->get_game_resume())
				{
					pause_menu->close();
					game->resume();
					pause_menu.reset();
				}
				else if (!pause_menu->get_game_resume() && pause_menu->is_closed())
				{
					game.reset();
					main_menu.reset(new MainMenu());
					pause_menu.reset();
				}
			}

			else if (game->is_over())
			{
				game.reset();
				main_menu.reset(new MainMenu());
			}
			else
			{
				do_action(e);
			}
		}
		if (toc(boshi_timer) / 1000.0 > 5.0 / glob::FPS)
		{
			tic(boshi_timer);
			boshi_split = (boshi_split + 1) % glob::TITLE_TEXT.length();
		}
		if (toc(render_timer) / 1000.0 > 1.0/glob::FPS)
		{
			tic(render_timer);
			render();
		}
		if (game.get() != nullptr && toc(game_timer) / 1000.0 > 1.0 / glob::GAMETICKS)
		{
			tic(game_timer);
			game->tick();
		}
	}

	close();
	return 0;
}