#pragma once
#include <math.h>

struct Vec2;

struct Mat22
{
private:
	float elements[2][2] = { {0.0f, 0.0f}, {0.0f, 0.0f } };

public:
	Mat22();
	Mat22(float x1, float x2, float x3, float x4);
	Mat22(float m[2][2]);

	Mat22 T();
	float norm();

	float operator()(int row, int col) const;

	Mat22 operator+(const Mat22 &mat) const;
	Mat22 operator-(const Mat22 &mat) const;
	Mat22 operator*(const Mat22 &mat) const;

	Mat22 &operator+=(const Mat22 &mat);
	Mat22 &operator-=(const Mat22 &mat);
	Mat22 &operator*=(const Mat22 &mat);

	bool operator==(const Mat22 &mat) const;

	Vec2 operator*(const Vec2 &v) const;

	Mat22 operator+(float &c) const;
	Mat22 operator-(float &c) const;
	Mat22 operator*(float &c) const;
	Mat22 operator/(float &c) const;
};
