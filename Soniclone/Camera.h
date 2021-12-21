#pragma once
#include "Vec2.h"


class Camera
{
public:
	Camera() = default;
	Camera(Vec2 &origin);
	~Camera();

	void setOrigin(Vec2 &origin);
	Vec2 getOrigin();

private:
	Vec2 *_origin = new Vec2(0.0f, 0.0f);
};

