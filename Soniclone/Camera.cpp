#include "Camera.h"

Camera::Camera(Vec2 &origin)
	:_origin(&origin) {}

Camera::~Camera() {}

void Camera::setOrigin(Vec2 &origin) { _origin = &origin; }
Vec2 Camera::getOrigin() { return Vec2(*_origin); }

Mat22 Camera::getTransform()
{
	float phi = -theta * M_PI / 180.0f;
	return rotMat(phi) * zoomMat(rx, ry);
}