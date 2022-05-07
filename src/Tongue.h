#pragma once
#include <array>
#include <math.h>
#include <SDL.h>
#include "globals.h"
#include "Entity.h"
#include "Vec2.h"

enum class TongueState
{
	IDLE,
	SHOT,
	ANCHORED,
	RELEASED
};

class Tongue
{
	Vec2 *tip = nullptr;
	Vec2 *end = nullptr;
	Vec2 *anchr = nullptr;
	Vec2 *origin = nullptr;

	float k = 0.005f;
	float rest_l = 20.0f;
	float d = 0.01f;
	float s_mass = 0.1;
	
	float shoot_speed = 10.0f;
	int reel = 0;

	TongueState state = TongueState::IDLE;

	void correctPos();
	void correctAngles();

	void reelOut();
	void reelIn();

public:
	std::array<Entity *, 12> parts{};
	BaseSprite *tongueEnd;

	Tongue(Vec2 *origin);
	~Tongue();

	TongueState getState();
	Entity* getTip();
	int getReel();

	Vec2 springForce(Vec2 &disp, Vec2 &vel, float m, bool half);

	void shoot(Vec2 const &dir, Vec2 const &r_vel);
	void release();
	void anchor();
	void anchor(Vec2 &pos);
	void idle();
	void teleport(Vec2 &v);
	void push(Vec2 &f);
	void applyGravity(float g);
	void update();
	void render(SDL_Renderer *renderer, Vec2 const &orig = Vec2(0.0f, 0.0f), Vec2 const &offset = Vec2(0.0f, 0.0f), float phi = 0.0f, float zx = 1.0f, float zy = 1.0f);

};

