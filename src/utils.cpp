#include "utils.h"

SDL_Surface *load_surface(std::string path, SDL_PixelFormatEnum pixel_format)
{

	SDL_Surface *loaded_surface = IMG_Load(path.c_str());
	if (loaded_surface == NULL)
	{
		std::cout << "Unable to load image " << path << " ! SDL_image Error: " << IMG_GetError() << "\n";
		return NULL;
	}

	//Convert surface to screen format
	SDL_Surface *optimized_surface = SDL_ConvertSurface(loaded_surface, SDL_AllocFormat(pixel_format), 0);
	if (optimized_surface == NULL)
	{
		std::cout << "Unable to optimize image " << path << " ! SDL Error: " << SDL_GetError() << "\n";
		return NULL;
	}

	SDL_FreeSurface(loaded_surface);
	return optimized_surface;
}

SDL_Texture *load_texture(std::string path, SDL_PixelFormatEnum pixel_format)
{
	SDL_Texture *new_texture = NULL;
	SDL_Surface *loaded_surface = IMG_Load(path.c_str());
	if (loaded_surface == NULL)
	{
		printf("Unable to load image %s! SDL_image Error: %s\n", path.c_str(), IMG_GetError());
		return NULL;
	}
	//Create texture from surface pixels
	loaded_surface = SDL_ConvertSurface(loaded_surface, SDL_AllocFormat(pixel_format), 0);
	new_texture = SDL_CreateTextureFromSurface(glob::g_renderer, loaded_surface);
	if (new_texture == NULL)
	{
		printf("Unable to create texture from %s! SDL Error: %s\n", path.c_str(), SDL_GetError());
	}

	SDL_FreeSurface(loaded_surface);
	return new_texture;
}

std::string debug_mode_string()
{
	switch (glob::DEBUG_MODE)
	{
	case DEBUG_OFF:
		return "OFF";

	case DEBUG_INFO:
		return "INFO";

	case DEBUG_DRAW:
		return "DRAW";

	case DEBUG_ALL:
		return "ALL";
	};
}