#pragma once
#include <string>
#include <string_view>
#include <SDL.h>
#include <SDL_ttf.h>
#include "DebugMode.h"
#define BREAKPOINT __nop()

namespace glob
{
	//Screen dimension inline constexprants
	inline constexpr int SCREEN_WIDTH{ 640 };
	inline constexpr int SCREEN_HEIGHT{ 480 };

	//Strings
	inline constexpr std::string_view TITLE_TEXT{ "BOOOSHI ROOOLL!! " };

	//Resource paths
	inline const std::string TITLE_IMG{ "resources/images/title.bmp" };
	inline const std::string BACKGROUND_IMG{ "resources/images/background.png" };
	inline const std::string FOREGROUND_IMG {"resources/images/foreground.png"};
	inline const std::string BOSHI_IMG {"resources/images/boshi.png"};
	inline const std::string BOSHI_IMG_BMP {"resources/images/boshi.bmp"};
	inline const std::string YOSHI_KART {"resources/images/yoshi_kart.bmp"};
	inline const std::string TONGUE_TIP {"resources/images/tongue_tip.bmp"};
	inline const std::string TONGUE_BODY {"resources/images/tongue_body.bmp"};
	inline const std::string TONGUE_END {"resources/images/tongue_end.bmp"};
	inline const std::string BUTTON_UP {"resources/images/button_up.png"};
	inline const std::string BUTTON_OVER {"resources/images/button_over.png"};
	inline const std::string BUTTON_DOWN {"resources/images/button_down.png"};
	inline const std::string COMIC_FONT {"resources/fonts/comic.ttf"};
	inline const std::string COMIC_FONT_BOLD {"resources/fonts/comicbd.ttf"};
	inline const std::string COMIC_FONT_ITALIC {"resources/fonts/comici.ttf"};


	//Timers
	inline constexpr int FPS{ 60 };
	inline constexpr int GAMETICKS{ 120 };

	//SDL stuff
	extern SDL_Window *g_window;
	extern SDL_Renderer *g_renderer;
	extern SDL_Surface *g_screen;

	//Debug config
	extern DebugMode DEBUG_MODE;
};