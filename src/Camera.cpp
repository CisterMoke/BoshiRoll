#include "Camera.h"

Camera::Camera(Vec2 &origin)
	:_origin(&origin) {}

Camera::~Camera() {}

void Camera::set_origin(const Vec2 &origin) { _origin = &origin; }
Vec2 Camera::get_origin() const { return Vec2(*_origin); }
Vec2 Camera::get_pixel_origin() const { return Vec2(*_origin).to_pixels(); }

Mat22 Camera::get_transform() const
{
	float phi = -theta * M_PI / 180.0f;
	return rotMat(phi) * zoomMat(rx, ry);
}