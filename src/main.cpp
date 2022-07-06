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
using namespace glob;


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
bool loadMedia();
void close();

//Main surfaces and textures
SDL_Window *glob::gWindow = nullptr;
SDL_Renderer *glob::gRenderer = nullptr;
SDL_Surface *glob::gScreen = nullptr;
SDL_Surface *gTitleImg = nullptr;
SDL_Texture *gTitleText = nullptr;

SDL_Surface *gSurf = nullptr;
SDL_Texture *gTexture = nullptr;

SDL_Surface *gBoshiSurfs[ORIENT_TOTAL];
SDL_Texture *gBoshiTexts[ORIENT_TOTAL];
AnimSprite *yoshiKart = new AnimSprite();
FontSprite *titleFont;
FontSprite *debugFont;

std::shared_ptr<Player> Boshi;
std::shared_ptr<Level> mainLvl;
Game game;
Camera cam = Camera();
SDL_Color collor = { 180, 0, 180 };
DebugMode debug = DEBUG_ALL;

bool boshiFlag = true;
std::stringstream boshiText;
int boshiSplit = 0;

bool init()
{
	// Init root
	if (SDL_Init(SDL_INIT_EVERYTHING) < 0)
	{
		std::cout << "SDL could not initialize! SDL_Error: " << SDL_GetError() << "\n";
		return false;
	}

	//Create window
	gWindow = SDL_CreateWindow("BoshiRoll", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_OPENGL);
	if (gWindow == NULL)
	{
		std::cout << "Window could not be created!" << SDL_GetError() << "\n";
		return false;
	}
	//Create renderer for window
	gRenderer = SDL_CreateRenderer(gWindow, -1, SDL_RENDERER_ACCELERATED);
	if (gRenderer == NULL)
	{
		printf("Renderer could not be created! SDL Error: %s\n", SDL_GetError());
		return false;
	}
	//Initialize renderer color
	SDL_SetRenderDrawColor(gRenderer, 0xFF, 0xFF, 0xFF, 0xFF);

	// Init png loading
	int imgFlags = IMG_INIT_PNG;
	if (!(IMG_Init(imgFlags) & imgFlags))
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

	gScreen = SDL_GetWindowSurface(gWindow);
	SDL_FillRect(gScreen, NULL, SDL_MapRGB(gScreen->format, 0xFF, 0xFF, 0xFF));
	SDL_UpdateWindowSurface(gWindow);
	mainLvl = std::make_shared<Level>();
	return true;
}

TTF_Font *loadFont(std::string path, int size)
{
	TTF_Font *font = TTF_OpenFont(path.c_str(), size);
	if (font == NULL)
	{
		std::cout << "Unable to load font " << path << std::endl;
		return NULL;
	}
	return font;
}

bool loadMedia()
{
	mainLvl->setBackground(BACKGROUND_IMG);
	mainLvl->setForeground(FOREGROUND_IMG);
	yoshiKart->loadFromFile(YOSHI_KART, { 32, 32 }, ALPHA | COLORKEY);
	yoshiKart->zoom(3, 3);
	gTitleImg = loadSurface(TITLE_IMG);
	if (gTitleImg == NULL)
	{
		return false;
	}

	gTitleText = SDL_CreateTextureFromSurface(gRenderer, gTitleImg);
	SDL_SetRenderDrawColor(gRenderer, 255, 255, 255, 0 );

	titleFont = new FontSprite(loadFont(COMIC_FONT_BOLD, 54));
	titleFont->setColor({ 22, 26, 255 });

	debugFont = new FontSprite(loadFont(COMIC_FONT_BOLD, 16));


	return true;
}

void loadGame()
{
	std::shared_ptr<RectCollider>  rectlu = std::make_shared<RectCollider>(
		new Vec2(0.0f, 0.0f), yoshiKart->getWidth(), yoshiKart->getHeight());
	std::shared_ptr<RectCollider>  rectld = std::make_shared<RectCollider>(
		new Vec2(0.0f, SCREEN_HEIGHT - yoshiKart->getHeight()), yoshiKart->getWidth(), yoshiKart->getHeight());
	std::shared_ptr<RectCollider>  rectru = std::make_shared<RectCollider>(
		new Vec2(SCREEN_WIDTH - yoshiKart->getWidth(), 0.0f), yoshiKart->getWidth(), yoshiKart->getHeight());
	std::shared_ptr<RectCollider>  rectrd = std::make_shared<RectCollider>(
		new Vec2(SCREEN_WIDTH - yoshiKart->getWidth(), SCREEN_HEIGHT - yoshiKart->getHeight()), yoshiKart->getWidth(), yoshiKart->getHeight());
	std::shared_ptr<LineCollider>  line = std::make_shared<LineCollider>(
		new Vec2(-SCREEN_WIDTH, SCREEN_HEIGHT * 0.8f), new Vec2(2 * SCREEN_WIDTH, SCREEN_HEIGHT * 0.6f));
	std::shared_ptr<LineCollider>  line2 = std::make_shared<LineCollider>(
		new Vec2(-10.5 * SCREEN_WIDTH, SCREEN_HEIGHT * 0.75f), new Vec2(20.5 * SCREEN_WIDTH, SCREEN_HEIGHT * 0.75f));
	std::shared_ptr<RampCollider>  ramp = std::make_shared<RampCollider>(
		new Vec2(1600.0f, SCREEN_HEIGHT * 0.75f-250), 250, Quadrant::IV);


	mainLvl->spawn = Vec2(SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2);
	mainLvl->colliders.push_back(rectlu);
	mainLvl->colliders.push_back(rectld);
	mainLvl->colliders.push_back(rectru);
	mainLvl->colliders.push_back(rectrd);
	//mainLvl->colliders.push_back(line);
	mainLvl->colliders.push_back(line2);
	mainLvl->colliders.push_back(ramp);

	Boshi = std::make_shared<Player>(BOSHI_IMG_BMP, 0.3, 0x03);
	*Boshi->pos = mainLvl->spawn;
	game = Game(Boshi, mainLvl);
	cam.setOrigin(*Boshi->pos);
	cam.rx = 0.5, cam.ry = 0.5;
}

void close()
{
	SDL_DestroyTexture(gTexture);
	gTexture = nullptr;
	SDL_FreeSurface(gTitleImg);
	gTitleImg = nullptr;
	SDL_FreeSurface(gSurf);
	gSurf = nullptr;

	SDL_DestroyRenderer(gRenderer);
	SDL_DestroyWindow(gWindow);
	gWindow = nullptr;
	gRenderer = nullptr;

	//Quit SDL subsystems
	IMG_Quit();
	SDL_Quit();
}

void tic(unsigned int *t) { *t = SDL_GetTicks(); }

unsigned int toc(unsigned int *t) { return SDL_GetTicks() - *t; }

void doAction(SDL_Event &event)
{
	if (event.type == SDL_KEYDOWN)
	{
		switch (event.key.keysym.sym)
		{
		case SDLK_SPACE:
			boshiFlag = !boshiFlag;
			if (boshiFlag)
			{
				yoshiKart->setFrame(0);
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
			*Boshi->pos = Vec2(SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2);
			Boshi->stop();
			break;
		case SDLK_F3:
			if (debug == DEBUG_OFF) { debug = DEBUG_ALL; }
			else { debug = DEBUG_OFF; }
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

	if (boshiFlag)
	{
		Boshi->doAction(event);
	}
}

float title_angle = 0.0f;
void render()
{
	if (!boshiFlag)
	{
		title_angle -= 0.5;
		SDL_RenderClear(gRenderer);
		boshiText.str("");
		boshiText << TITLE_TEXT.substr(boshiSplit) << TITLE_TEXT.substr(0, boshiSplit);
		titleFont->setText(boshiText.str());
		titleFont->renderAt(0, 0, Vec2(SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2), title_angle);
	}

	else
	{
		Vec2 orig = cam.getOrigin();
		Vec2 off = Vec2(SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2);
		mainLvl->renderBackground();
		Boshi->render(gRenderer, orig, off, cam.theta, cam.rx, cam.ry);
		int yw = yoshiKart->getWidth() / 2;
		int yh = yoshiKart->getHeight() / 2;
		yoshiKart->renderAt(Vec2(yw - orig.x, yh - orig.y), off, cam.theta, cam.rx, cam.ry);
		yoshiKart->renderAt(Vec2(yw - orig.x, SCREEN_HEIGHT - yh - orig.y), off, cam.theta, cam.rx, cam.ry);
		yoshiKart->toggleFlip(SDL_FLIP_HORIZONTAL);
		yoshiKart->renderAt(Vec2(SCREEN_WIDTH - yw - orig.x, yh - orig.y), off, cam.theta, cam.rx, cam.ry);
		yoshiKart->renderAt(Vec2(SCREEN_WIDTH - yw - orig.x, SCREEN_HEIGHT - yh - orig.y), off, cam.theta, cam.rx, cam.ry);
		yoshiKart->toggleFlip(SDL_FLIP_HORIZONTAL);
		yoshiKart->sync(true);
		mainLvl->renderForeground(orig, off, Vec2(cam.rx, cam.ry));
	}

	if ((debug & DEBUG_INFO) == DEBUG_INFO) { renderText(Boshi.get(), debugFont); }
	if ((debug & DEBUG_DRAW) == DEBUG_DRAW)
	{
		for (auto collider : mainLvl->colliders)
		{
			drawCollider(gRenderer, collider.get(), &cam, Vec2(SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2));
		}
		drawCollider(gRenderer, Boshi->collider, &cam, Vec2(SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2));
		if (Boshi->tongue->getState() != TongueState::IDLE)
		{
			drawCollider(gRenderer, Boshi->tongue->parts[0]->collider, &cam, Vec2(SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2));
		}
	}
	SDL_SetRenderDrawColor(gRenderer, 255, 255, 255, 255);
	SDL_RenderPresent(gRenderer);
}

int main(int argc, char *argv[])
{
	bool quit = false;
	SDL_Event e;
	unsigned int *renderTimer = new unsigned int(0);
	unsigned int *gameTimer = new unsigned int(0);
	unsigned int *boshiTimer = new unsigned int(0);
	tic(renderTimer);
	tic(gameTimer);
	tic(boshiTimer);

	SDL_SetMainReady();
	init();
	if (!loadMedia())
	{
		std::cout << "Failed to load media!\n";
		return 0;
	}
	loadGame();
	while (!quit)
	{
		while (SDL_PollEvent(&e) != 0)
		{
			if (e.type == SDL_QUIT)
			{
				quit = true;
				break;
			}
			else 
			{
				doAction(e);
			}
		}
		if (toc(boshiTimer) / 1000.0 > 5.0 / FPS)
		{
			tic(boshiTimer);
			boshiSplit = (boshiSplit + 1) % TITLE_TEXT.length();
		}
		if (toc(renderTimer) / 1000.0 > 1.0/FPS)
		{
			tic(renderTimer);
			render();
		}
		if (toc(gameTimer) / 1000.0 > 1.0 / GAMETICKS)
		{
			tic(gameTimer);
			game.tick();
		}
	}


	close();
	return 0;
}