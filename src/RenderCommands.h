#pragma once
#include <SDL.h>
#include<SDL2_gfxPrimitives.h>
#include <algorithm>
#include <deque>
#include "Camera.h"
#include "globals.h"
#include "Mat22.h"
#include "Sprites.h"
#include "Vec2.h"

struct BaseRenderCommand
{
	Camera *camera = nullptr;
	virtual ~BaseRenderCommand() {};
	virtual void execute(SDL_Renderer *renderer) = 0;
};

extern std::deque<BaseRenderCommand *> render_command_pool;

template <class Sprite>
struct RenderSpriteCommand : public BaseRenderCommand
{
	Sprite *sprite;
	Vec2 pos;
	SDL_Rect *clip;
	bool center;

	RenderSpriteCommand(Sprite *sprite, Camera *camera = nullptr, Vec2 pos = Vec2(0.0f, 0.0f), bool center = true, SDL_Rect *clip = NULL)
		: sprite(sprite), pos(pos), clip(clip), center(center)
	{
		this->camera = camera;
	}

	virtual ~RenderSpriteCommand() {};
	virtual void execute(SDL_Renderer *renderer);

protected:
	void render_sprite(SDL_Renderer *renderer);
	void _render_sprite_cam(SDL_Renderer *renderer);
	void _render_sprite_no_cam(SDL_Renderer *renderer);
};

template<class Sprite>
void RenderSpriteCommand<Sprite>::render_sprite(SDL_Renderer *renderer)
{
	if (sprite->get_texture() == nullptr) { return; }
	camera == NULL ? _render_sprite_no_cam(renderer) : _render_sprite_cam(renderer);
}

template<class Sprite>
void RenderSpriteCommand<Sprite>::_render_sprite_cam(SDL_Renderer *renderer)
{
	float w_hat = sprite->get_width() * camera->rx;
	float h_hat = sprite->get_height() * camera->ry;
	Vec2 off = Vec2((glob::SCREEN_WIDTH - w_hat) / 2, (glob::SCREEN_HEIGHT - h_hat) / 2);
	if (!center) { pos += Vec2(sprite->get_width() / 2, sprite->get_height() / 2); }
	Vec2 lu = camera->get_transform() * (pos - camera->get_origin()) + off;
	SDL_Rect dest = { lu.x, lu.y, w_hat, h_hat };
	SDL_RenderCopyEx(renderer, sprite->get_texture(), clip, &dest, sprite->get_theta() - camera->theta, NULL, sprite->get_flip());
}


template<class Sprite>
void RenderSpriteCommand<Sprite>::_render_sprite_no_cam(SDL_Renderer *renderer)
{
	float w_hat = sprite->get_width();
	float h_hat = sprite->get_height();
	Vec2 off = Vec2(0.0f, 0.0f);
	if (center) { off -= Vec2(w_hat / 2, h_hat / 2); }
	Vec2 lu = pos + off;
	SDL_Rect dest = { lu.x, lu.y, w_hat, h_hat };
	SDL_RenderCopyEx(renderer, sprite->get_texture(), clip, &dest, sprite->get_theta(), NULL, sprite->get_flip());
}

struct RenderParallaxCommand : public RenderSpriteCommand<BaseSprite>
{
	float depth;
	RenderParallaxCommand(BaseSprite *sprite, Camera *camera = nullptr, Vec2 pos = Vec2(0.0f, 0.0f), float depth = 1.0f, bool center = false);
	virtual ~RenderParallaxCommand() {};
	virtual void execute(SDL_Renderer *renderer);
};



template<class Collider>
struct RenderColliderCommand : public BaseRenderCommand
{
	Collider *coll;
	SDL_Color color;
	RenderColliderCommand(Collider *coll, Camera *camera = nullptr, SDL_Color color = { 180, 0, 180 })
		: coll(coll), color(color)
	{
		this->camera = camera;
	}

	virtual ~RenderColliderCommand() {};
	virtual void execute(SDL_Renderer *renderer);
};