#pragma once
#include "Vec2.h"
#include "Mat22.h"


class Camera
{
public:
	float rx = 1.0f;
	float ry = 1.0f;
	float theta = 0.0f;
	Camera() = default;
	Camera(Vec2 &origin);
	~Camera();

	void setOrigin(Vec2 &origin);
	Vec2 getOrigin();

	Mat22 getTransform();

private:
	Vec2 *_origin = new Vec2(0.0f, 0.0f);
};

