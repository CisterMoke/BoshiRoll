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

	px_w = base_surf->w;
	px_h = base_surf->h;

	if ((mode & ALPHA) != ALPHA) { base_surf = std::shared_ptr<SDL_Surface>(SDL_ConvertSurfaceFormat(base_surf.get(), SDL_PIXELFORMAT_RGB888, 0), SDL_FreeSurface); }
	if ((mode & COLORKEY) == COLORKEY) { SDL_SetColorKey(base_surf.get(), SDL_TRUE, SDL_MapRGB(base_surf->format, 0xFF, 0x0, 0xFF)); }
	texture = std::shared_ptr<SDL_Texture>(SDL_CreateTextureFromSurface(glob::g_renderer, base_surf.get()), SDL_DestroyTexture);
	if (texture == NULL)
	{
		return false;
	}
	return true;
}

int BaseSprite::get_pixel_width() const
{
	return scale_x * px_w;
}

float BaseSprite::get_width() const
{
	return (float) get_pixel_width() / glob::PX_PER_M;
}

int BaseSprite::get_pixel_height() const
{
	return scale_y * px_h;
}

float BaseSprite::get_height() const
{
	return (float) get_pixel_height() / glob::PX_PER_M;
}

float BaseSprite::get_theta() const
{
	return theta;
}

float BaseSprite::get_degrees() const
{
	return -fmodf(fmodf(theta * 180.0f / M_PI, 360) + 360, 360);
}

Vec2 BaseSprite::get_zoom() const
{
	return Vec2(scale_x, scale_y);
}

SDL_Texture *BaseSprite::get_texture()
{
	return texture.get();
}

SDL_RendererFlip BaseSprite::get_flip() const
{
	return _flip;
}

void BaseSprite::reset()
{
	clear_mem();
	px_w = 0;
	px_h = 0;
	theta = 0.0;
	scale_x = 1.0;
	scale_y = 1.0;
	_flip = SDL_FLIP_NONE;
}

void BaseSprite::revert()
{
	px_w = base_surf->w;
	px_h = base_surf->h;
	theta = 0.0;
	scale_x = 1.0;
	scale_y = 1.0;
	_flip = SDL_FLIP_NONE;

	texture.reset(SDL_CreateTextureFromSurface(glob::g_renderer, base_surf.get()), SDL_DestroyTexture);
}

void BaseSprite::set_pixel_width(int width) { px_w = width; }
void BaseSprite::set_pixel_height(int height) { px_h = height; }
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