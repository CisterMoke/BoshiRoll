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
#include "BaseSprite.h"
#include "AnimSprite.h"
#include "FontSprite.h"
#include "Colliders.h"
#include "Entity.h"
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
TTF_Font *glob::gFont = nullptr;
SDL_Surface *gBackImg = nullptr;
SDL_Texture *gBackText = nullptr;

SDL_Surface *gSurf = nullptr;
SDL_Texture *gTexture = nullptr;

SDL_Surface *gBoshiSurfs[ORIENT_TOTAL];
SDL_Texture *gBoshiTexts[ORIENT_TOTAL];
AnimSprite *yoshiKart = new AnimSprite();
FontSprite *titleFont = new FontSprite();

std::shared_ptr<Entity> Boshi;
std::shared_ptr<Level> mainLvl;
Game game;
Camera cam = Camera();
SDL_Color collor = { 180, 0, 180 };

bool boshiFlag = false;
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
	gWindow = SDL_CreateWindow("Soniclone", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_OPENGL);
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
	return true;
}

bool loadMedia()
{
	yoshiKart->loadFromFile(YOSHI_KART, { 32, 32 }, 0x03);
	yoshiKart->zoom(3, 3);
	gBackImg = loadSurface(BACKGROUND_IMG);
	if (gBackImg == NULL)
	{
		return false;
	}

	gBackText = SDL_CreateTextureFromSurface(gRenderer, gBackImg);
	SDL_SetRenderDrawColor(gRenderer, 255, 255, 255, 0 );

	SDL_Surface *boshiSurf = loadSurface(BOSHI_IMG, SDL_PIXELFORMAT_RGBA8888);
	if (boshiSurf == NULL)
	{
		return false;
	}

	glob::gFont = TTF_OpenFont(COMIC_FONT_BOLD.c_str(), 54);
	if (glob::gFont == NULL)
	{
		std::cout << "Unable to load font " << COMIC_FONT_BOLD << std::endl;
		return false;
	}
	titleFont->setFont(glob::gFont);
	titleFont->setColor({ 22, 26, 255 });
	titleFont->setText("BOSHI SPIN!!!");


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
		new Vec2(0.0f, SCREEN_HEIGHT), new Vec2(SCREEN_WIDTH, SCREEN_HEIGHT - 450.0f));

	mainLvl = std::make_shared<Level>();
	mainLvl->spawn = Vec2(SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2);
	mainLvl->colliders.push_back(rectlu);
	mainLvl->colliders.push_back(rectld);
	mainLvl->colliders.push_back(rectru);
	mainLvl->colliders.push_back(rectrd);
	mainLvl->colliders.push_back(line);

	Boshi = std::make_shared<Entity>(BOSHI_IMG_BMP, 0.3, 0x03);
	*Boshi->pos = mainLvl->spawn;
	game = Game(Boshi, mainLvl);
	cam.setOrigin(*Boshi->pos);
}

void close()
{
	SDL_DestroyTexture(gTexture);
	gTexture = nullptr;
	SDL_FreeSurface(gBackImg);
	gBackImg = nullptr;
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

		case SDLK_i:
			cam.ry /= 1.02;
			break;

		case SDLK_k:
			cam.ry *= 1.02;
			break;

		case SDLK_j:
			cam.rx /= 1.02;
			break;

		case SDLK_l:
			cam.rx *= 1.02;
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
			*Boshi->pos = Vec2(SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2);
			Boshi->stop();
		}
	}

	if (boshiFlag)
	{
		Boshi->doAction(event);
	}
}

void render()
{
	SDL_RenderClear(gRenderer);
	boshiText.str("");
	boshiText << TITLE_TEXT.substr(boshiSplit) << TITLE_TEXT.substr(0, boshiSplit);
	titleFont->setText(boshiText.str());
	titleFont->renderAt(SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2);

	if (boshiFlag)
	{
		Vec2 orig = cam.getOrigin();
		Vec2 off = Vec2(SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2);
		Mat22 T = cam.getTransform();
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
		for (auto collider : mainLvl->colliders)
		{
			collider->draw(gRenderer, collor, orig, off, T);
		}
		Boshi->collider->draw(gRenderer, collor, orig, off, T);
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