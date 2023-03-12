#pragma once
#include <SDL.h>
#include <string>
#include <math.h>
#include <memory>
#include "Vec2.h"
#include "BaseSprite.h"
#include "Colliders.h"

class Entity
{
public:
	Vec2 pos = Vec2(0.0f, 0.0f);
	Vec2 vel = Vec2(0.0f, 0.0f);
	Vec2 t_force = Vec2(0.0f, 0.0f);
	float theta = 0.0f;
	float omega = 0.0f; // Clockwise
	float r_force = 0.0f;
	float inertia = 0.5f; // "reduced" I_hat = I/(r^2) with I the actual inertia.
	float mass = 1.0;
	std::shared_ptr<BaseSprite> sprite;
	std::unique_ptr<BaseCollider> collider;

	Entity(std::shared_ptr<BaseSprite> sprite, const Vec2 &pos = Vec2(0.0f, 0.0f));
	Entity(std::shared_ptr<BaseSprite> sprite, float x, float y);

	virtual ~Entity() = 0;

	void set_theta(float angle);
	void teleport(Vec2 const &v);
	void push(Vec2 const &f);
	void rotate(float angle);
	void spin(float alpha);
	void stop();
	void apply_velocities();
	void apply_forces();
	void update();
};