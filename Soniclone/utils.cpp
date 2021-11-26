#include "utils.h"
using glob::gRenderer;

SDL_Surface *loadSurface(std::string path, SDL_PixelFormatEnum pixelFormat)
{

	SDL_Surface *loadedSurface = IMG_Load(path.c_str());
	if (loadedSurface == NULL)
	{
		std::cout << "Unable to load image " << path << " ! SDL_image Error: " << IMG_GetError() << "\n";
		return NULL;
	}

	//Convert surface to screen format
	SDL_Surface *optimizedSurface = SDL_ConvertSurface(loadedSurface, SDL_AllocFormat(pixelFormat), 0);
	if (optimizedSurface == NULL)
	{
		std::cout << "Unable to optimize image " << path << " ! SDL Error: " << SDL_GetError() << "\n";
		return NULL;
	}

	SDL_FreeSurface(loadedSurface);
	return optimizedSurface;
}

SDL_Texture *loadTexture(std::string path, SDL_PixelFormatEnum pixelFormat)
{
	SDL_Texture *newTexture = NULL;
	SDL_Surface *loadedSurface = IMG_Load(path.c_str());
	if (loadedSurface == NULL)
	{
		printf("Unable to load image %s! SDL_image Error: %s\n", path.c_str(), IMG_GetError());
		return NULL;
	}
	//Create texture from surface pixels
	loadedSurface = SDL_ConvertSurface(loadedSurface, SDL_AllocFormat(pixelFormat), 0);
	newTexture = SDL_CreateTextureFromSurface(gRenderer, loadedSurface);
	if (newTexture == NULL)
	{
		printf("Unable to create texture from %s! SDL Error: %s\n", path.c_str(), SDL_GetError());
	}

	SDL_FreeSurface(loadedSurface);
	return newTexture;
}