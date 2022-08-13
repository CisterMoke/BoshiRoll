#pragma once
#include <math.h>
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
	BaseCollider() = default;
	virtual ~BaseCollider() {};
	bool virtual check_collision(CircleCollider &c, Vec2 *cptr = nullptr) = 0;
	Vec2 virtual collision_disp(CircleCollider &c, Vec2 *cptr = nullptr) = 0;
	virtual BaseRenderCommand *create_cmd(Camera *camera = nullptr, SDL_Color color = {180, 0, 180}) = 0;
};



struct CircleCollider : public BaseCollider
{
	Vec2 *pos = new Vec2(0.0f, 0.0f);
	float r = 1.0f;

	CircleCollider();
	CircleCollider(Vec2 *p, float radius);
	CircleCollider(float x, float y, float radius);
	~CircleCollider();
	bool check_collision(CircleCollider &c, Vec2 *cptr = nullptr);
	Vec2 collision_disp(CircleCollider &c, Vec2 *cptr = nullptr);
	Vec2 anti_collision_disp(CircleCollider &c, Vec2 *cptr = nullptr);
	virtual BaseRenderCommand *create_cmd(Camera *camera = nullptr, SDL_Color color = { 180, 0, 180 });
};

struct LineCollider : public BaseCollider
{
	Vec2 *start = new Vec2(0.0f, 0.0f);
	Vec2 *stop = new Vec2(0.0f, 0.0f);
	LineCollider();
	LineCollider(Vec2 *start, Vec2 *stop);
	~LineCollider();
	bool check_collision(CircleCollider &c, Vec2 *cptr = nullptr);
	Vec2 collision_disp(CircleCollider &c, Vec2 *cptr = nullptr);
	virtual BaseRenderCommand *create_cmd(Camera *camera = nullptr, SDL_Color color = { 180, 0, 180 });
};

struct RectCollider : public BaseCollider
{
	Vec2 *pos = new Vec2(0.0f, 0.0f); // LU corner
	float w = 0.0f, h = 0.0f;

	RectCollider();
	RectCollider(Vec2 *p, float width, float height);
	RectCollider(float x, float y, float width, float height);
	~RectCollider();
	bool check_collision(CircleCollider &c, Vec2 *cptr = nullptr);
	bool check_collision(RectCollider &r);
	Vec2 collision_disp(CircleCollider &c, Vec2 *cptr = nullptr);
	Vec2 collision_disp(RectCollider &r);
	virtual BaseRenderCommand *create_cmd(Camera *camera = nullptr, SDL_Color color = { 180, 0, 180 });
};

struct RampCollider : public BaseCollider
{
	Vec2* pos = new Vec2(0.0f, 0.0f); // LU corner
	float sz = 0.0f;
	Quadrant quad = Quadrant::IV;
	CircleCollider *circ;
	RectCollider *rect;
	LineCollider *line;

	RampCollider();
	RampCollider(Vec2 *p, float size, Quadrant quadrant);
	RampCollider(float x, float y, float size, Quadrant quadrant);
	~RampCollider();
	bool check_collision(CircleCollider &c, Vec2 *cptr = nullptr);
	Vec2 collision_disp(CircleCollider &c, Vec2 *cptr = nullptr);
	virtual BaseRenderCommand *create_cmd(Camera *camera = nullptr, SDL_Color color = { 180, 0, 180 });
};
