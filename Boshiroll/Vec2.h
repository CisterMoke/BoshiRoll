#pragma once
#include <math.h>
#include <iostream>
#ifndef _USE_MATH_DEFINES
#define _USE_MATH_DEFINES
#endif
#include "Mat22.h"
struct Vec2
{
	float x, y;

	Vec2(float x, float y);
	Vec2(const Vec2 &v);

	float dot(const Vec2 &v) const;
	float cross(const Vec2 &v) const;
	Vec2 cross(float z) const;
	float dist(const Vec2 &v) const;
	float norm() const;
	float norm2() const;

	Vec2 normalize();

	friend std::ostream &operator<<(std::ostream &os, const Vec2 &v);

	Vec2 operator+(const Vec2 &v) const;
	Vec2 operator-(const Vec2 &v) const;
	Vec2 operator*(const Vec2 &v) const;
	Vec2 operator/(const Vec2 &v) const;

	Vec2 operator+(float c) const;
	Vec2 operator-(float c) const;
	Vec2 operator*(float c) const;
	Vec2 operator/(float c) const;

	Vec2 operator*(const Mat22 &mat) const;

	bool operator==(const Vec2 &v) const;
	bool operator<(const Vec2 &v) const;
	bool operator>(const Vec2 &v) const;
	bool operator<=(const Vec2 &v) const;
	bool operator>=(const Vec2 &v) const;

	Vec2 &operator+=(const Vec2 &v);
	Vec2 &operator-=(const Vec2 &v);
	Vec2 &operator*=(const Vec2 &v);
	Vec2 &operator/=(const Vec2 &v);

	Vec2 &operator+=(float c);
	Vec2 &operator-=(float c);
	Vec2 &operator*=(float c);
	Vec2 &operator/=(float c);

	Vec2 &operator*=(const Mat22 &mat);
};