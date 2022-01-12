#pragma once
#include <array>
#include <math.h>
#include <SDL.h>
#include "globals.h"
#include "Entity.h"
#include "Vec2.h"

enum TongueState
{
	IDLE = 0,
	SHOT = 1,
	ANCHORED = 2,
	RELEASED = 3
};

class Tongue
{
	Vec2 *tip = nullptr;
	Vec2 *end = nullptr;
	Vec2 *anchr = nullptr;
	Vec2 *origin = nullptr;

	float k = 0.15f;
	float rest_l = 5.0f;
	float d = 0.1f;


	float shoot_speed = 10.0f;
	int reel = 0;
	float reel_len = 20.0f;

	TongueState state = IDLE;

	void correctPos();
	void correctAngles();

	void reel_out();
	void reel_in();

public:
	std::array<Entity *, 19> parts{};
	BaseSprite *tongueEnd;

	Tongue(Vec2 *origin);
	~Tongue();

	TongueState getState();

	Vec2 springForce(Vec2 &disp, Vec2 &vel);
	Vec2 bottomForce(Vec2 &pos, Vec2 &vel);

	void shoot(Vec2 const &dir);
	void release();
	void anchor(Vec2 &pos);
	void idle();
	void teleport(Vec2 &v);
	void push(Vec2 &f);
	void update();
	void render(SDL_Renderer *renderer, Vec2 const &orig = Vec2(0.0f, 0.0f), Vec2 const &offset = Vec2(0.0f, 0.0f), float phi = 0.0f, float zx = 1.0f, float zy = 1.0f);

};

