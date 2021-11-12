#pragma once
#include <SDL.h>
#include <string>
#include <math.h>
#include "Vec2.h"
#include "BaseSprite.h"
#include "CircleCollider.h"

class Entity
{
public:
	Vec2 *pos = new Vec2(0.0f, 0.0f);
	Vec2 *vel = new Vec2(0.0f, 0.0f);
	Vec2 *t_force = new Vec2(0.0f, 0.0f);
	float *theta;
	float omega = 0.0f;
	float r_force = 0.0f;
	BaseSprite *sprite;
	CircleCollider *collider;

	Entity(std::string spritePath, float zoom = 1.0f, int mode = 0x01);
	Entity(std::string spritePath, Vec2 *pos, float zoom = 1.0f, int mode = 0x01);
	Entity(std::string spritePath, float x, float y, float zoom = 1.0f, int mode = 0x01);

	~Entity();

	void move(Vec2 const &v);
	void rotate(float angle);
	void update();
	void render(SDL_Renderer *renderer, Vec2 const &offset = Vec2(0.0f, 0.0f));
	void doAction(SDL_Event &event);

private:
	void applyTForce();
	void applyTFriction();
	void applyRForce();
	void applyRFriction();

};

