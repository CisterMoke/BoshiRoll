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

	void correct_pos();
	void correct_angles();

	void reel_out();
	void reel_in();

public:
	std::array<Entity *, 12> parts{};
	BaseSprite *tongue_end;

	Tongue(Vec2 *origin);
	~Tongue();

	TongueState get_state();
	Entity* get_tip();
	int get_reel();

	Vec2 spring_force(Vec2 &disp, Vec2 &vel, float m, bool half);

	void shoot(Vec2 const &dir, Vec2 const &r_vel);
	void release();
	void anchor();
	void anchor(Vec2 &pos);
	void idle();
	void teleport(Vec2 &v);
	void push(Vec2 &f);
	void apply_gravity(float g);
	void update();

};

