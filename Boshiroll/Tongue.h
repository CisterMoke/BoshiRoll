#pragma once
#include <array>
#include <SDL.h>
#include "globals.h"
#include "Entity.h"
#include "Vec2.h"

enum TongueState
{
	IDLE = 0,
	SHOOT = 1,
	GRAPPLE = 2,
	RELEASE = 3
};

class Tongue
{
	std::array<Entity, 5> parts;
	Vec2 *tip = nullptr;
	Vec2 *end = nullptr;

	float shootSpeed = 0.2f;
	Vec2 shootDir = Vec2(0.0f, 0.0f);

	TongueState state = IDLE;

public:
	Tongue();
	~Tongue();

	void shoot(Vec2 const &dir);
	void release();
	void attach(Vec2 *anchor);
	void update();

};

