//Using SDL and standard IO
#define SDL_MAIN_HANDLED
#include <SDL.h>
#include <SDL_image.h>
#include <SDL_ttf.h>
#include <memory>
#include <iostream>
#include <sstream>
#include "globals.h"
#include "utils.h"
#include "debug.h"
#include "BaseSprite.h"
#include "AnimSprite.h"
#include "FontSprite.h"
#include "Colliders.h"
#include "Player.h"
#include "Level.h"
#include "Game.h"
#include "Camera.h"
#include "Renderer.h"
#include "Menu.h"


enum Orientation
{
	UP,
	DOWN,
	LEFT,
	RIGHT,
	ORIENT_TOTAL,
};

//Main gameloop functions
bool init();
bool load_media();
void close();

//Main surfaces and textures
SDL_Surface *g_title_img = nullptr;
SDL_Texture *g_title_text = nullptr;

SDL_Surface *g_surf = nullptr;
SDL_Texture *g_texture = nullptr;

SDL_Surface *g_boshi_surfs[ORIENT_TOTAL];
SDL_Texture *g_boshi_texts[ORIENT_TOTAL];
Renderer renderer;
AnimSprite *yoshi_kart = new AnimSprite();
AnimSprite *yoshi_kart2 = nullptr;
FontSprite *title_font;
FontSprite *debug_font;

std::unique_ptr<MainMenu> main_menu;

std::shared_ptr<Player> boshi;
std::shared_ptr<Level> main_lvl;
std::unique_ptr<Game> game;
Camera cam = Camera();
SDL_Color collor = { 180, 0, 180 };

bool boshi_flag = true;
std::stringstream boshiText;
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
	main_lvl = std::make_shared<Level>();
	return true;
}

bool load_media()
{
	main_lvl->set_background(glob::BACKGROUND_IMG, Vec2(640.0f/2880*1.5f, 480.0f/1920*1.5f));
	main_lvl->set_foreground(glob::FOREGROUND_IMG, Vec2(0.5f, 0.5f));
	yoshi_kart->load_from_file(glob::YOSHI_KART, { 32, 32 }, ALPHA | COLORKEY);
	yoshi_kart->zoom(3, 3);
	yoshi_kart2 = new AnimSprite(*yoshi_kart);
	yoshi_kart2->flip(SDL_FLIP_HORIZONTAL);
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
	std::shared_ptr<RectCollider>  rectlu = std::make_shared<RectCollider>(
		new Vec2(0.0f, 0.0f), yoshi_kart->get_width(), yoshi_kart->get_height());
	std::shared_ptr<RectCollider>  rectld = std::make_shared<RectCollider>(
		new Vec2(0.0f, glob::SCREEN_HEIGHT - yoshi_kart->get_height()), yoshi_kart->get_width(), yoshi_kart->get_height());
	std::shared_ptr<RectCollider>  rectru = std::make_shared<RectCollider>(
		new Vec2(glob::SCREEN_WIDTH - yoshi_kart->get_width(), 0.0f), yoshi_kart->get_width(), yoshi_kart->get_height());
	std::shared_ptr<RectCollider>  rectrd = std::make_shared<RectCollider>(
		new Vec2(glob::SCREEN_WIDTH - yoshi_kart->get_width(), glob::SCREEN_HEIGHT - yoshi_kart->get_height()), yoshi_kart->get_width(), yoshi_kart->get_height());
	std::shared_ptr<LineCollider>  line = std::make_shared<LineCollider>(
		new Vec2(-glob::SCREEN_WIDTH, glob::SCREEN_HEIGHT * 0.8f), new Vec2(2 * glob::SCREEN_WIDTH, glob::SCREEN_HEIGHT * 0.6f));
	std::shared_ptr<LineCollider>  line2 = std::make_shared<LineCollider>(
		new Vec2(-10.5 * glob::SCREEN_WIDTH, glob::SCREEN_HEIGHT * 0.75f), new Vec2(20.5 * glob::SCREEN_WIDTH, glob::SCREEN_HEIGHT * 0.75f));
	std::shared_ptr<RampCollider>  ramp = std::make_shared<RampCollider>(
		new Vec2(1600.0f, glob::SCREEN_HEIGHT * 0.75f-250), 250, Quadrant::IV);


	main_lvl->spawn = Vec2(glob::SCREEN_WIDTH / 2, glob::SCREEN_HEIGHT / 2);
	main_lvl->colliders.push_back(rectlu);
	main_lvl->colliders.push_back(rectld);
	main_lvl->colliders.push_back(rectru);
	main_lvl->colliders.push_back(rectrd);
	//main_lvl->colliders.push_back(line);
	main_lvl->colliders.push_back(line2);
	main_lvl->colliders.push_back(ramp);

	boshi = std::make_shared<Player>(glob::BOSHI_IMG_BMP, 0.3, 0x03);
	game.reset(new Game(boshi, main_lvl));
	game->camera = &cam;
	game->init();
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
		case SDLK_SPACE:
			boshi_flag = !boshi_flag;
			cam.theta = 0;
			if (boshi_flag)
			{
				yoshi_kart->set_frame(0);
			}
			break;

		case SDLK_ESCAPE:
			SDL_Event e;
			e.type = SDL_QUIT;
			SDL_PushEvent(&e);

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
			*boshi->pos = Vec2(glob::SCREEN_WIDTH / 2, glob::SCREEN_HEIGHT / 2);
			boshi->stop();
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

	if (boshi_flag)
	{
		boshi->do_action(event);
	}
}

float title_angle = 0.0f;
void render()
{
	if (main_menu != nullptr)
	{
		title_font->rotate_tot(1.5f);
		SDL_RenderClear(glob::g_renderer);
		boshiText.str("");
		boshiText << glob::TITLE_TEXT.substr(boshi_split) << glob::TITLE_TEXT.substr(0, boshi_split);
		title_font->set_text(boshiText.str());
		render_command_pool.push_back(new RenderSpriteCommand<FontSprite>(title_font, NULL, Vec2(glob::SCREEN_WIDTH/2, glob::SCREEN_HEIGHT/2)));
		main_menu->push_render_cmds();
		
	}

	else if (game != nullptr)
	{
		Vec2 orig = cam.get_origin();
		Vec2 off = Vec2(glob::SCREEN_WIDTH / 2, glob::SCREEN_HEIGHT / 2);
		int yw = yoshi_kart->get_width() / 2;
		int yh = yoshi_kart->get_height() / 2;
		render_command_pool.push_back(new RenderSpriteCommand<AnimSprite>(yoshi_kart, &cam, Vec2(yw, yh)));
		render_command_pool.push_back(new RenderSpriteCommand<AnimSprite>(yoshi_kart, &cam, Vec2(yw,glob::SCREEN_HEIGHT - yh)));
		render_command_pool.push_back(new RenderSpriteCommand<AnimSprite>(yoshi_kart2, &cam, Vec2(glob::SCREEN_WIDTH - yw, yh)));
		render_command_pool.push_back(new RenderSpriteCommand<AnimSprite>(yoshi_kart2, &cam, Vec2(glob::SCREEN_WIDTH - yw, glob::SCREEN_HEIGHT - yh)));
		game->push_render_commands();

		if ((glob::DEBUG_MODE & DEBUG_DRAW) == DEBUG_DRAW)
		{
			for (auto &collider : main_lvl->colliders)
			{
				render_command_pool.push_back(collider->create_cmd(&cam));
			}

			render_command_pool.push_back(boshi->collider->create_cmd(&cam));

			if (boshi->tongue->get_state() != TongueState::IDLE)
			{
				render_command_pool.push_back(boshi->tongue->parts[0]->collider->create_cmd(&cam));
			}
		}

		if ((glob::DEBUG_MODE & DEBUG_INFO) == DEBUG_INFO) {
			push_entity_info_render_command(boshi.get(), debug_font);
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
			else if (main_menu != nullptr)
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
				}
			}

			else if (game->over)
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
		if (game != nullptr && toc(game_timer) / 1000.0 > 1.0 / glob::GAMETICKS)
		{
			tic(game_timer);
			game->tick();
		}
	}

	close();
	return 0;
}