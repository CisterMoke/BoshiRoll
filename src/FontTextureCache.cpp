#include "FontTextureCache.h"



FontTextureCache::FontTextureCache(const FontTextureCache &other)
{
	for (int i = 0; i < textures.size(); i++)
	{
		if (other.contains(i))
		{
			textures[i] = other.textures[i];
			dimensions[i] = std::make_unique<const Vec2>(*other.dimensions[i]);
		}
	}
}

FontTextureCache &FontTextureCache::operator=(const FontTextureCache &other)
{
	if (this == &other)
		return *this;

	for (int i = 0; i < textures.size(); i++)
	{
		if (other.contains(i))
		{
			textures[i] = other.textures[i];
			dimensions[i] = std::make_unique<const Vec2>(*other.dimensions[i]);
		}
	}
	return *this;
}

std::shared_ptr<SDL_Texture> &FontTextureCache::get_texture(char c)
{
	return textures[c];
}

const Vec2 *FontTextureCache::get_dim(char c)
{
	return dimensions[c].get();
}

void FontTextureCache::insert(char c, std::shared_ptr<SDL_Texture> texture, const Vec2 &dim)
{
	textures[c] = texture;
	dimensions[c] = std::make_unique<const Vec2>(dim);
}

void FontTextureCache::clear()
{
	for (int i = 0; i < textures.size(); i++)
	{
		textures[i].reset();
		dimensions[i].reset();
	}
}

bool FontTextureCache::contains(char c) const
{
	if (c < 0 || c >= textures.size()) { return false; }
	return !(dimensions[c].get() == nullptr);
}