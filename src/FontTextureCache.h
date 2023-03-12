#pragma once
#include <SDL.h>
#include <array>
#include <memory>
#include <utility>
#include "Vec2.h"

class FontTextureCache
{
private:
	std::array<std::shared_ptr<SDL_Texture>, 127> textures;
	std::array<std::unique_ptr<const Vec2>, 127> dimensions;

public:
	FontTextureCache() = default;
	~FontTextureCache() = default;
	FontTextureCache(const FontTextureCache &other);
	FontTextureCache &operator=(const FontTextureCache &other);

	std::shared_ptr<SDL_Texture> &get_texture(char c);
	const Vec2 *get_dim(char c);

	void insert(char c, std::shared_ptr<SDL_Texture> texture, const Vec2& dim);
	void clear();
	bool contains(char c) const;
};