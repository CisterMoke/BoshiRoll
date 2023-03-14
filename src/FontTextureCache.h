#pragma once
#include <array>
#include <memory>
#include <utility>
#include <SDL.h>
#include "Vec2.h"

class FontTextureCache
{
private:
	std::array<std::shared_ptr<SDL_Texture>, 127> textures{};
	std::array<Vec2, 127> dimensions{};
	std::array<bool, 127> flags{};


public:

	const std::shared_ptr<SDL_Texture> &get_texture(char c) const;
	const Vec2 get_dim(char c) const;

	void insert(char c, std::shared_ptr<SDL_Texture> texture, const Vec2& dim);
	void clear();
	bool contains(char c) const;
};