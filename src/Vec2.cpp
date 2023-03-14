#include "Vec2.h"

Vec2::Vec2(float x, float y)
	: x(x), y(y) {}

float Vec2::dot(const Vec2 &v) const
{
	return x * v.x + y * v.y;
}

float Vec2::cross(const Vec2 &v) const
{
	return x * v.y - y * v.x;
}

Vec2 Vec2::cross(float z) const
{
	// Return cross product of self with z unit vector
	return Vec2(y * z, - x * z);
}

float Vec2::dist(const Vec2 &v) const
{
	return sqrtf(pow(x - v.x, 2) + pow(y - v.y, 2));
}

float Vec2::norm() const { return sqrtf(x * x + y * y); }
float Vec2::norm2() const { return x * x + y * y; }

Vec2 Vec2::normalize() const { return *this / this->norm(); }

std::ostream &operator<<(std::ostream &os, const Vec2 &v)
{
	os << "(" << v.x << ", " << v.y << ")";
	return os;
}

Vec2 Vec2::operator+(const Vec2 &v) const { return Vec2(x + v.x, y + v.y); }
Vec2 Vec2::operator-(const Vec2 &v) const { return Vec2(x - v.x, y - v.y); }
Vec2 Vec2::operator*(const Vec2 &v) const { return Vec2(x * v.x, y * v.y); }
Vec2 Vec2::operator/(const Vec2 &v) const { return Vec2(x / v.x, y / v.y); }

Vec2 Vec2::operator+(float c) const { return Vec2(x + c, y + c); }
Vec2 Vec2::operator-(float c) const { return Vec2(x - c, y - c); }
Vec2 Vec2::operator*(float c) const { return Vec2(x * c, y * c); }
Vec2 Vec2::operator/(float c) const { return Vec2(x / c, y / c); }

Vec2 Vec2::operator*(const Mat22 &mat) const
{
	float a = x * mat(0, 0) + y * mat(1, 0);
	float b = x * mat(0, 1) + y * mat(1, 1);
	return Vec2(a, b);
}

bool Vec2::operator==(const Vec2 &v) const { return x == v.x && y == v.y; }
bool Vec2::operator<(const Vec2 &v) const { return this->norm2() < v.norm2(); }
bool Vec2::operator>(const Vec2 &v) const { return this->norm2() > v.norm2(); }
bool Vec2::operator<=(const Vec2 &v) const { return this->norm2() <= v.norm2(); }
bool Vec2::operator>=(const Vec2 &v) const { return this->norm2() >= v.norm2(); }

Vec2 &Vec2::operator+=(const Vec2 &v)
{
	x += v.x;
	y += v.y;
	return *this;
}
Vec2 &Vec2::operator-=(const Vec2 &v)
{
	x -= v.x;
	y -= v.y;
	return *this;
}
Vec2 &Vec2::operator*=(const Vec2 &v)
{
	x *= v.x;
	y *= v.y;
	return *this;
}
Vec2 &Vec2::operator/=(const Vec2 &v)
{
	x /= v.x;
	y /= v.y;
	return *this;
}
Vec2 &Vec2::operator+=(float c)
{
	x += c;
	y += c;
	return *this;
}
Vec2 &Vec2::operator-=(float c)
{
	x -= c;
	y -= c;
	return *this;
}
Vec2 &Vec2::operator*=(float c)
{
	x *= c;
	y *= c;
	return *this;
}
Vec2 &Vec2::operator/=(float c)
{
	x /= c;
	y /= c;
	return *this;
}

Vec2 &Vec2::operator*=(const Mat22 &mat)
{
	float a = x * mat(0, 0) + y * mat(0, 1);
	float b = x * mat(1, 0) + y * mat(1, 1);
	x = a;
	y = b;
	return *this;
}