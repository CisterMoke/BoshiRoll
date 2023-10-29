#pragma once
#include <math.h>
#include <iostream>
#include <box2d.h>
#ifndef _USE_MATH_DEFINES
#define _USE_MATH_DEFINES
#endif
#include "globals.h"
#include "Mat22.h"
struct Vec2
{
	float x, y; // Keep order to maintain conversion to b2Vec2

	Vec2() :x(0), y(0) {};
	Vec2(float x, float y);

	float dot(const Vec2 &v) const;
	float cross(const Vec2 &v) const;
	Vec2 cross(float z) const;
	float dist(const Vec2 &v) const;
	float norm() const;
	float norm2() const;

	Vec2 normalize() const;
	Vec2 to_pixels() const;

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

	operator b2Vec2&() const;
};