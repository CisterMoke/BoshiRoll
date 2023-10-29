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

	void set_origin(const Vec2 &origin);
	Vec2 get_origin() const;
	Vec2 get_pixel_origin() const;

	Mat22 get_transform() const;

private:
	const Vec2 *_origin = new Vec2(0.0f, 0.0f);
};

