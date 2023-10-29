#pragma once
#include <memory>
#include <string>
#include <SDL.h>
#include <SDL_image.h>
#include "globals.h"
#include "utils.h"
#include "Vec2.h"

enum LoadMode
{
	ALPHA = 0x01,
	COLORKEY = 0x02
};

class BaseSprite
{
	friend class Entity;
public:
	BaseSprite(std::string path, int mode = ALPHA | COLORKEY);

	bool load_from_file(std::string path, int mode = ALPHA | COLORKEY);

	int get_pixel_width() const;
	float get_width() const;
	int get_pixel_height() const;
	float get_height() const;
	float get_theta() const;
	float get_degrees() const;
	Vec2 get_zoom() const;
	SDL_Texture *get_texture();
	SDL_RendererFlip get_flip() const;

	virtual void reset();
	void revert();
	void zoom(float rx, float ry);
	void flip(SDL_RendererFlip f);
	void toggle_flip(SDL_RendererFlip f);
	void rotate(float angle);
	void set_pixel_width(int width);
	void set_pixel_height(int height);
	void set_theta(float angle);

protected:
	BaseSprite() = default;
	void clear_mem();

	int px_w{ 0 };
	int px_h{ 0 };
	float theta{ 0.0 };
	float scale_x{ 1.0 };
	float scale_y{ 1.0 };
	SDL_RendererFlip _flip{ SDL_FLIP_NONE };

	std::shared_ptr<SDL_Texture> texture{};
	std::shared_ptr<SDL_Surface> base_surf{};

};

