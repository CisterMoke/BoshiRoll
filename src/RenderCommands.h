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

#include "RenderCommands.tpp"