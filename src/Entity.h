#pragma once
#include <string>
#include <math.h>
#include <memory>
#include <SDL.h>
#include <box2d.h>
#include "Vec2.h"
#include "BaseSprite.h"
#include "Colliders.h"

struct EntityData
{
	const Vec2 &pos;
	const Vec2 &vel;
	const float theta;
	const float omega;
	const float mass;
	const float inertia;

	EntityData(b2Body &body);
};

class Entity
{
protected:
	b2World *world;
	b2Body *body;
	virtual b2Body *add_to(b2World &world, const Vec2 &pos) = 0;

public:
	//Vec2 vel{ 0.0f, 0.0f };
	//Vec2 t_force{ 0.0f, 0.0f };
	//float omega{ 0.0f }; // Clockwise
	//float r_force{ 0.0f };
	//float inertia{ 0.5f }; // "reduced" I_hat = I/(r^2) with I the actual inertia.
	float density{ 1.0 };
	float mass{ 0.0f };
	float inertia{ 0.0f };
	std::shared_ptr<BaseSprite> sprite;

	Entity(std::shared_ptr<BaseSprite> sprite, b2World &world, const Vec2 &pos = {0.0f, 0.0f});

	virtual ~Entity() = 0;

	EntityData get_data() const;
	const Vec2 &get_pos() const;
	float get_theta() const;
	void set_theta(float angle);
	float get_size() const;
	void grow(float scale);


	void teleport(Vec2 const &v);
	void push(Vec2 const &f);
	void rotate(float angle);
	void spin(float alpha);
	void stop();
	//void apply_velocities();
	//void apply_forces();
	void update();
};