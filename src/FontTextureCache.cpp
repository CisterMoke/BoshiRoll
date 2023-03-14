#include "FontTextureCache.h"


const std::shared_ptr<SDL_Texture> &FontTextureCache::get_texture(char c) const
{
	return textures[c];
}

const Vec2 FontTextureCache::get_dim(char c) const
{
	return dimensions[c];
}

void FontTextureCache::insert(char c, std::shared_ptr<SDL_Texture> texture, const Vec2 &dim)
{
	textures[c] = texture;
	dimensions[c] = dim;
	flags[c] = true;
}

void FontTextureCache::clear()
{
	flags.fill(false);
	dimensions.fill(Vec2());
	for (int i = 0; i < textures.size(); i++)
	{
		textures[i].reset();
	}
}

bool FontTextureCache::contains(char c) const
{
	if (c < 0 || c >= textures.size()) { return false; }
	return flags[c];
}