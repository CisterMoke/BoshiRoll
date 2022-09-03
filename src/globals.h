#pragma once
#include <string>
#include <SDL.h>
#include <SDL_ttf.h>
#include "DebugMode.h"
#define BREAKPOINT __nop()

struct  glob
{
	//Screen dimension constants
	static const int SCREEN_WIDTH;
	static const int SCREEN_HEIGHT;

	//Strings
	static const std::string TITLE_TEXT;

	//Resource paths
	static const std::string TITLE_IMG;
	static const std::string BACKGROUND_IMG;
	static const std::string FOREGROUND_IMG;
	static const std::string BOSHI_IMG;
	static const std::string BOSHI_IMG_BMP;
	static const std::string YOSHI_KART;
	static const std::string TONGUE_TIP;
	static const std::string TONGUE_BODY;
	static const std::string TONGUE_END;
	static const std::string BUTTON_UP;
	static const std::string BUTTON_OVER;
	static const std::string BUTTON_DOWN;
	static const std::string COMIC_FONT;
	static const std::string COMIC_FONT_BOLD;
	static const std::string COMIC_FONT_ITALIC;


	//Timers
	static const int FPS;
	static const int GAMETICKS;

	//SDL stuff
	static SDL_Window *g_window;
	static SDL_Renderer *g_renderer;
	static SDL_Surface *g_screen;

	//Debug config
	static DebugMode DEBUG_MODE;
};