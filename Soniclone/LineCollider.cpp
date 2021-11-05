#include "LineCollider.h"

LineCollider::LineCollider() {};
LineCollider::LineCollider(Vec2 *start, Vec2 *stop)
	: start(start), stop(stop) {};
LineCollider::~LineCollider()
{
	delete start, stop;
}
