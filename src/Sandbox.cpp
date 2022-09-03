#define SDL_MAIN_HANDLED
#include <SDL.h>
#include <iostream>
#include "globals.h"
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
bool load_media();
void close();

//Main surfaces and textures
SDL_Window *glob::g_window = nullptr;
SDL_Renderer *glob::g_renderer = nullptr;
SDL_Surface *glob::g_screen = nullptr;
SDL_Surface *g_title_img = nullptr;
SDL_Texture *g_title_text = nullptr;

SDL_Surface *g_surf = nullptr;
SDL_Texture *g_texture = nullptr;

SDL_Surface *g_boshi_surfs[ORIENT_TOTAL];
SDL_Texture *g_boshi_texts[ORIENT_TOTAL];


#include "ButtonActions.h"
#include "MenuButton.h"
#include "Menu.h"
int ref = 0;
std::vector<int> arr = { 0, 2, 4 };

void foo(MenuButton *b, int x, int y)
{
	b->press_at(x, y);
	b->release_at(x, y);
	std::cout << ref << std::endl;
}

int main(int argc, char *argv[])
{
	SDL_SetMainReady();
	std::unique_ptr<int> ptr = std::unique_ptr<int>(nullptr);
	std::cout << (ptr == nullptr) << (ptr.get() == nullptr) << std::endl;
	ptr = nullptr;
	std::cout << (ptr == nullptr) << (ptr.get() == nullptr) << std::endl;
	ptr = std::make_unique<int>(3);
	std::cout << (ptr == nullptr) << (ptr.get() == nullptr) << std::endl;
}