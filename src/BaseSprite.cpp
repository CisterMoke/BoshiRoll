#include "BaseSprite.h"

BaseSprite::BaseSprite(std::string path, int mode)
{
	load_from_file(path, mode);
}

bool BaseSprite::load_from_file(std::string path, int mode)
{
	reset();

	base_surf = std::shared_ptr<SDL_Surface>(load_surface(path, SDL_PIXELFORMAT_RGBA8888), SDL_FreeSurface);
	if (base_surf == NULL)
	{
		return false;
	}

	w = base_surf->w;
	h = base_surf->h;

	if ((mode & ALPHA) != ALPHA) { base_surf = std::shared_ptr<SDL_Surface>(SDL_ConvertSurfaceFormat(base_surf.get(), SDL_PIXELFORMAT_RGB888, 0), SDL_FreeSurface); }
	if ((mode & COLORKEY) == COLORKEY) { SDL_SetColorKey(base_surf.get(), SDL_TRUE, SDL_MapRGB(base_surf->format, 0xFF, 0x0, 0xFF)); }
	texture = std::shared_ptr<SDL_Texture>(SDL_CreateTextureFromSurface(glob::g_renderer, base_surf.get()), SDL_DestroyTexture);
	if (texture == NULL)
	{
		return false;
	}
	return true;
}

int BaseSprite::get_width() { return scale_x * w; }
int BaseSprite::get_height() { return scale_y * h; }
float BaseSprite::get_theta() { return theta; }
SDL_Texture *BaseSprite::get_texture() { return texture.get(); }
SDL_RendererFlip BaseSprite::get_flip() { return _flip; }

void BaseSprite::reset()
{
	clear_mem();
	w = 0;
	h = 0;
	theta = 0.0;
	scale_x = 1.0;
	scale_y = 1.0;
	_flip = SDL_FLIP_NONE;
}

void BaseSprite::revert()
{
	w = base_surf->w;
	h = base_surf->h;
	theta = 0.0;
	scale_x = 1.0;
	scale_y = 1.0;
	_flip = SDL_FLIP_NONE;

	texture.reset(SDL_CreateTextureFromSurface(glob::g_renderer, base_surf.get()), SDL_DestroyTexture);
}

void BaseSprite::set_width(int width) { w = width; }
void BaseSprite::set_height(int height) { h = height; }
void BaseSprite::set_theta(float angle) { theta = angle; }
void BaseSprite::zoom(float rx, float ry)
{
	scale_x  = rx;
	scale_y = ry;
}

void BaseSprite::flip(SDL_RendererFlip f) { _flip = f; }
void BaseSprite::toggle_flip(SDL_RendererFlip f) { _flip =(SDL_RendererFlip)(_flip ^ f); }
void BaseSprite::rotate(float angle) { theta = fmodf(fmodf(theta + angle, 360) + 360, 360); }

void BaseSprite::clear_mem()
{
	base_surf.reset();
	texture.reset();
}