#pragma once
#include <string>
#include <SDL.h>
#include <SDL_ttf.h>
#define BREAKPOINT __nop()
namespace glob
{
	//Screen dimension constants
	const int SCREEN_WIDTH = 640;
	const int SCREEN_HEIGHT = 480;

	//Strings
	const std::string TITLE_TEXT = "BOOOSHI ROOOLL!! ";

	//Resource paths
	const std::string BACKGROUND_IMG = "resources/images/background.bmp";
	const std::string BOSHI_IMG = "resources/images/boshi.png";
	const std::string BOSHI_IMG_BMP = "resources/images/boshi.bmp";
	const std::string YOSHI_KART = "resources/images/yoshi_kart.bmp";
	const std::string TONGUE_TIP = "resources/images/tongue_tip.bmp";
	const std::string TONGUE_BODY = "resources/images/tongue_body.bmp";
	const std::string TONGUE_END = "resources/images/tongue_end.bmp";
	const std::string COMIC_FONT = "resources/fonts/comic.ttf";
	const std::string COMIC_FONT_BOLD = "resources/fonts/comicbd.ttf";
	const std::string COMIC_FONT_ITALIC = "resources/fonts/comici.ttf";

	//Timers
	const int FPS = 60;
	const int GAMETICKS = 120;

	//SDL stuff
	extern SDL_Window *gWindow;
	extern SDL_Renderer *gRenderer;
	extern SDL_Surface *gScreen;
}