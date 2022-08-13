#include "Camera.h"

Camera::Camera(Vec2 &origin)
	:_origin(&origin) {}

Camera::~Camera() {}

void Camera::set_origin(Vec2 &origin) { _origin = &origin; }
Vec2 Camera::get_origin() { return Vec2(*_origin); }

Mat22 Camera::get_transform()
{
	float phi = -theta * M_PI / 180.0f;
	return rotMat(phi) * zoomMat(rx, ry);
}