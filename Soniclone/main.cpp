//Using SDL and standard IO
#define SDL_MAIN_HANDLED
#include <SDL.h>
#include<SDL_image.h>
#include <SDL2_rotozoom.h>
#include <iostream>
#include "globals.h"
#include "utils.h"
#include "BaseSprite.h"
#include "AnimSprite.h"
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
SDL_Window *glob::gWindow = NULL;
SDL_Renderer *glob::gRenderer = NULL;
SDL_Surface *glob::gScreen = NULL;
SDL_Surface *gBackImg = NULL;
SDL_Texture *gBackText = NULL;

SDL_Surface *gSurf = NULL;
SDL_Texture *gTexture = NULL;

SDL_Surface *gBoshiSurfs[ORIENT_TOTAL];
SDL_Texture *gBoshiTexts[ORIENT_TOTAL];
BaseSprite *boshiSprite = new BaseSprite();
AnimSprite *yoshiKart = new AnimSprite();
bool boshiFlag = false;


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

	SDL_Surface *boshiSurf = loadSurface(BOSHI_IMG, SDL_PIXELFORMAT_RGBA8888);
	if (boshiSurf == NULL)
	{
		return false;
	}

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
				//boshiSprite->zoom(0.3, 0.3);
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
				boshiSprite->flip(SDL_FLIP_HORIZONTAL);
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

int main(int argc, char *argv[])
{
	bool quit = false;
	SDL_Event e;

	SDL_SetMainReady();
	init();
	if (!loadMedia())
	{
		std::cout << "Failed to load media!\n";
		return 0;
	}

	int animLag = 10;
	int animCount = 0;
	while (!quit)
	{
		animCount = (animCount + 1) % animLag;
		SDL_RenderClear(gRenderer);
		SDL_RenderCopy(gRenderer, gBackText, NULL, NULL);
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
		//SDL_RenderCopy(gRenderer, gTexture, NULL, NULL);
		if (boshiFlag)
		{
			boshiSprite->renderAt(SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2);
			int yw = yoshiKart->getWidth()/2;
			int yh = yoshiKart->getHeight()/2;
			yoshiKart->renderAt(yw, yh);
			yoshiKart->renderAt(yw, SCREEN_HEIGHT - yh);
			yoshiKart->renderAt(SCREEN_WIDTH - yw, yh);
			yoshiKart->renderAt(SCREEN_WIDTH - yw, SCREEN_HEIGHT - yh);
			if (!animCount)
			{
				yoshiKart->advance();
			}
		}
		SDL_RenderPresent(gRenderer);
		//SDL_UpdateWindowSurface(gWindow);
	}

	delete boshiSprite;

	close();
	return 0;
}