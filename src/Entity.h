#pragma once
#include <SDL.h>
#include <string>
#include <math.h>
#include "Vec2.h"
#include "BaseSprite.h"
#include "Colliders.h"

class Entity
{
public:
	Vec2 *pos = new Vec2(0.0f, 0.0f);
	Vec2 *vel = new Vec2(0.0f, 0.0f);
	Vec2 *t_force = new Vec2(0.0f, 0.0f);
	float *theta;
	float omega = 0.0f; // Clockwise
	float r_force = 0.0f;
	float inertia = 0.5f; // "reduced" I_hat = I/(r^2) with I the actual inertia.
	float mass = 1.0;
	BaseSprite *sprite;
	CircleCollider *collider;

	Entity(std::string sprite_path, float zoom = 1.0f, int mode = ALPHA | COLORKEY);
	Entity(std::string sprite_path, Vec2 &pos, float zoom = 1.0f, int mode = ALPHA | COLORKEY);
	Entity(std::string sprite_path, float x, float y, float zoom = 1.0f, int mode = ALPHA | COLORKEY);

	~Entity();

	void teleport(Vec2 const &v);
	void push(Vec2 const &f);
	void rotate(float angle);
	void spin(float alpha);
	void stop();
	void apply_velocities();
	void apply_forces();
	void update();
};