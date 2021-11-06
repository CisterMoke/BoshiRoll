//Using SDL and standard IO
#define SDL_MAIN_HANDLED
#include <SDL.h>
#include <SDL_image.h>
#include <SDL_ttf.h>
#include <iostream>
#include <sstream>
#include "globals.h"
#include "utils.h"
#include "BaseSprite.h"
#include "AnimSprite.h"
#include "FontSprite.h"
#include "Colliders.h"
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
BaseSprite *boshiSprite = new BaseSprite();
AnimSprite *yoshiKart = new AnimSprite();
FontSprite *titleFont = new FontSprite();

SDL_Color collor = { 180, 0, 180 };
LineCollider line = LineCollider(new Vec2(0.0f, SCREEN_HEIGHT), new Vec2(SCREEN_WIDTH, SCREEN_HEIGHT/2));
CircleCollider circ = CircleCollider();
RectCollider rect = RectCollider();

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
	boshiSprite->loadFromFile(BOSHI_IMG_BMP, 0x3);
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

	*circ.pos = Vec2(SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2);
	circ.r = boshiSprite->getWidth()/2;

	*rect.pos = Vec2(0.0f, 0.0f);
	rect.w = yoshiKart->getWidth();
	rect.h = yoshiKart->getHeight();

	return true;
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

void doAction(SDL_Event event)
{
	if (event.type == SDL_KEYDOWN)
	{
		switch (event.key.keysym.sym)
		{
		case SDLK_SPACE:
			boshiFlag = !boshiFlag;
			if (boshiFlag)
			{ 
				boshiSprite->revert();
				yoshiKart->setFrame(0);
			}
			break;

		case SDLK_ESCAPE:
			SDL_Event e;
			e.type = SDL_QUIT;
			SDL_PushEvent(&e);
		}

		if (boshiFlag)
		{
			switch (event.key.keysym.sym)
			{
			case SDLK_UP:
				boshiSprite->revert();
				break;

			case SDLK_DOWN:
				boshiSprite->toggleFlip(SDL_FLIP_HORIZONTAL);
				break;

			case SDLK_LEFT:
				boshiSprite->rotate(30);
				break;

			case SDLK_RIGHT:
				boshiSprite->rotate(-30);
				break;
			}
		}
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
		boshiSprite->renderAt(SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2);
		int yw = yoshiKart->getWidth() / 2;
		int yh = yoshiKart->getHeight() / 2;
		yoshiKart->renderAt(yw, yh);
		yoshiKart->renderAt(yw, SCREEN_HEIGHT - yh);
		yoshiKart->toggleFlip(SDL_FLIP_HORIZONTAL);
		yoshiKart->renderAt(SCREEN_WIDTH - yw, yh);
		yoshiKart->renderAt(SCREEN_WIDTH - yw, SCREEN_HEIGHT - yh);
		yoshiKart->toggleFlip(SDL_FLIP_HORIZONTAL);
		yoshiKart->sync(true);
	}
	debug::drawCollider(line, collor);
	debug::drawCollider(circ, collor);
	debug::drawCollider(rect, collor);
	SDL_SetRenderDrawColor(gRenderer, 255, 255, 255, 255);
	SDL_RenderPresent(gRenderer);
}

int main(int argc, char *argv[])
{
	bool quit = false;
	SDL_Event e;
	unsigned int *frameTimer = new unsigned int(0);
	unsigned int *boshiTimer = new unsigned int(0);
	tic(frameTimer);
	tic(boshiTimer);

	SDL_SetMainReady();
	init();
	if (!loadMedia())
	{
		std::cout << "Failed to load media!\n";
		return 0;
	}

	while (!quit)
	{
		while (SDL_PollEvent(&e) != 0)
		{
			if (e.type == SDL_QUIT)
			{
				quit = true;
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
		if (toc(frameTimer) / 1000.0 > 1.0/FPS)
		{
			tic(frameTimer);
			render();
		}
	}

	delete boshiSprite;

	close();
	return 0;
}