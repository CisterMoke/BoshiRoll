#include "globals.h"


//Screen dimension constants
const int glob::SCREEN_WIDTH = 640;
const int glob::SCREEN_HEIGHT = 480;

//Strings
const std::string glob::TITLE_TEXT = std::string("BOOOSHI ROOOLL!! ");

//Resource paths
const std::string glob::TITLE_IMG = "resources/images/title.bmp";
const std::string glob::BACKGROUND_IMG = "resources/images/background.png";
const std::string glob::FOREGROUND_IMG = "resources/images/foreground.png";
const std::string glob::BOSHI_IMG = "resources/images/boshi.png";
const std::string glob::BOSHI_IMG_BMP = "resources/images/boshi.bmp";
const std::string glob::YOSHI_KART = "resources/images/yoshi_kart.bmp";
const std::string glob::TONGUE_TIP = "resources/images/tongue_tip.bmp";
const std::string glob::TONGUE_BODY = "resources/images/tongue_body.bmp";
const std::string glob::TONGUE_END = "resources/images/tongue_end.bmp";
const std::string glob::BUTTON_UP = "resources/images/button_up.png";
const std::string glob::BUTTON_OVER = "resources/images/button_over.png";
const std::string glob::BUTTON_DOWN = "resources/images/button_down.png";
const std::string glob::COMIC_FONT = "resources/fonts/comic.ttf";
const std::string glob::COMIC_FONT_BOLD = "resources/fonts/comicbd.ttf";
const std::string glob::COMIC_FONT_ITALIC = "resources/fonts/comici.ttf";


//Timers
const int glob::FPS = 60;
const int glob::GAMETICKS = 120;

//SDL stuff
SDL_Window *glob::g_window = nullptr;
SDL_Renderer *glob::g_renderer = nullptr;
SDL_Surface *glob::g_screen = nullptr;

//Debug config
DebugMode glob::DEBUG_MODE = DEBUG_OFF;