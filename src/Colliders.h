#pragma once
#include <math.h>
#include <memory>
#include <SDL.h>
#include<SDL2_gfxPrimitives.h>
#include "RenderCommands.h"
#include "Camera.h"
#include "Vec2.h"
#include "Mat22.h"
#include "Quadrant.h"

struct CircleCollider;

struct BaseCollider
{
	std::shared_ptr<Vec2> pos = nullptr; // Center pos
	BaseCollider() = default;
	BaseCollider(std::shared_ptr<Vec2> p);

	bool virtual check_collision(CircleCollider &c, Vec2 *cptr = nullptr) const = 0;
	Vec2 virtual collision_disp(CircleCollider &c, Vec2 *cptr = nullptr) const = 0;
	virtual BaseRenderCommand *create_cmd(Camera *camera = nullptr, SDL_Color color = {180, 0, 180}) = 0;
};



struct CircleCollider : public BaseCollider
{
	float r = 1.0f;

	CircleCollider() = default;
	CircleCollider(std::shared_ptr<Vec2>  p, float radius);
	CircleCollider(float x, float y, float radius);

	bool check_collision(CircleCollider &c, Vec2 *cptr = nullptr) const;
	Vec2 collision_disp(CircleCollider &c, Vec2 *cptr = nullptr) const;
	Vec2 anti_collision_disp(CircleCollider &c, Vec2 *cptr = nullptr) const;
	virtual BaseRenderCommand *create_cmd(Camera *camera = nullptr, SDL_Color color = { 180, 0, 180 });
};

struct LineCollider : public BaseCollider
{
	std::shared_ptr<Vec2> start = std::shared_ptr<Vec2>(new Vec2(0.0f, 0.0f));
	std::shared_ptr<Vec2> stop = std::shared_ptr<Vec2>(new Vec2(0.0f, 0.0f));

	LineCollider() = default;
	LineCollider(std::shared_ptr<Vec2> start, std::shared_ptr<Vec2> stop);

	bool check_collision(CircleCollider &c, Vec2 *cptr = nullptr) const;
	Vec2 collision_disp(CircleCollider &c, Vec2 *cptr = nullptr) const;
	virtual BaseRenderCommand *create_cmd(Camera *camera = nullptr, SDL_Color color = { 180, 0, 180 });
};

struct RectCollider : public BaseCollider
{
	std::shared_ptr<Vec2> lu = std::shared_ptr<Vec2>(new Vec2(0.0f, 0.0f));
	float w = 0.0f, h = 0.0f;

	RectCollider() = default;
	RectCollider(std::shared_ptr<Vec2>  p, float width, float height);
	RectCollider(float x, float y, float width, float height);

	bool check_collision(CircleCollider &c, Vec2 *cptr = nullptr) const;
	bool check_collision(RectCollider &r) const;
	Vec2 collision_disp(CircleCollider &c, Vec2 *cptr = nullptr) const;
	Vec2 collision_disp(RectCollider &r) const;
	virtual BaseRenderCommand *create_cmd(Camera *camera = nullptr, SDL_Color color = { 180, 0, 180 });
};

struct RampCollider : public BaseCollider
{
	std::shared_ptr<Vec2> lu = std::shared_ptr<Vec2>(new Vec2(0.0f, 0.0f));
	float sz = 0.0f;
	Quadrant quad = Quadrant::IV;
	CircleCollider *circ;
	RectCollider *rect;
	LineCollider *line;

	RampCollider();
	RampCollider(std::shared_ptr<Vec2>  p, float size, Quadrant quadrant);
	RampCollider(float x, float y, float size, Quadrant quadrant);
	~RampCollider();
	bool check_collision(CircleCollider &c, Vec2 *cptr = nullptr) const;
	Vec2 collision_disp(CircleCollider &c, Vec2 *cptr = nullptr) const;
	virtual BaseRenderCommand *create_cmd(Camera *camera = nullptr, SDL_Color color = { 180, 0, 180 });
};
