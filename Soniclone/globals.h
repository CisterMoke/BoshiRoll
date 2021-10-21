#pragma once
#include <string>
#include <SDL.h>
namespace glob
{
	//Screen dimension constants
	const int SCREEN_WIDTH = 640;
	const int SCREEN_HEIGHT = 480;

	//Resource paths
	const std::string BACKGROUND_IMG = "resources/images/background.bmp";
	const std::string BOSHI_IMG = "resources/images/boshi.png";
	const std::string BOSHI_IMG_BMP = "resources/images/boshi.bmp";
	const std::string YOSHI_KART = "resources/images/yoshi_kart.bmp";


	//SDL stuff
	extern SDL_Window *gWindow;
	extern SDL_Renderer *gRenderer;
	extern SDL_Surface *gScreen;
}