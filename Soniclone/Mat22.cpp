#include "Mat22.h"
#include "Vec2.h"

Mat22::Mat22() {}
Mat22::Mat22(float x1, float x2, float x3, float x4)
{
	elements[0][0] = x1;
	elements[0][1] = x2;
	elements[1][0] = x3;
	elements[1][1] = x4;
}
Mat22::Mat22(float m[2][2])
{
	elements[0][0] = m[0][0];
	elements[0][1] = m[0][1];
	elements[1][0] = m[1][0];
	elements[1][1] = m[1][1];

}

Mat22 Mat22::T()
{
	return Mat22(elements[0][0], elements[1][0], elements[0][1], elements[1][1]);
}

float Mat22::norm()
{
	float sqSum = elements[0][0] * elements[0][0]
		+ elements[0][1] * elements[0][1]
		+ elements[1][0] * elements[1][0]
		+ elements[1][1] * elements[1][1];
	return sqrtf(sqSum);
}

float Mat22::operator()(int row, int col) const { return elements[row][col]; }

Mat22 Mat22::operator+(const Mat22 &mat) const
{
	float m[2][2] = {
		{elements[0][0] + mat(0, 0), elements[0][1] + mat(0, 1)},
		{elements[1][0] + mat(1, 0), elements[1][1] + mat(1, 1)} };
	return Mat22(mat);
}
Mat22 Mat22::operator-(const Mat22 &mat) const
{
	float m[2][2] = {
		{elements[0][0] - mat(0, 0), elements[0][1] - mat(0, 1)},
		{elements[1][0] - mat(1, 0), elements[1][1] - mat(1, 1)} };
	return Mat22(mat);
}
Mat22 Mat22::operator*(const Mat22 &mat) const
{
	float x1 = elements[0][0] * mat(0, 0) + elements[0][1] * mat(1, 0);
	float x2 = elements[0][0] * mat(0, 1) + elements[0][1] * mat(1, 1);
	float x3 = elements[1][0] * mat(0, 0) + elements[1][1] * mat(1, 0);
	float x4 = elements[1][0] * mat(0, 1) + elements[1][1] * mat(1, 1);
	return Mat22(mat);
}

Mat22 &Mat22::operator+=(const Mat22 &mat)
{
	*this = *this + mat;
	return *this;
}
Mat22 &Mat22::operator-=(const Mat22 &mat)
{
	*this = *this - mat;
	return *this;
}
Mat22 &Mat22::operator*=(const Mat22 &mat)
{
	*this = *this * mat;
	return *this;
}

bool Mat22::operator==(const Mat22 &mat) const
{
	return elements[0][0] == mat(0, 0) && elements[0][1] == mat(0, 1)
		&& elements[1][0] == mat(1, 0) && elements[1][1] == mat(1, 1);
}

Vec2 Mat22::operator*(const Vec2 &v) const
{
	return v * *this;
}


Mat22 Mat22::operator+(float &c) const
{ 
	return Mat22(elements[0][0] + c, elements[0][1] + c, elements[1][0] + c, elements[1][1] + c);
}
Mat22 Mat22::operator-(float &c) const
{
	return Mat22(elements[0][0] - c, elements[0][1] - c, elements[1][0] - c, elements[1][1] - c);
}
Mat22 Mat22::operator*(float &c) const
{
	return Mat22(elements[0][0] * c, elements[0][1] * c, elements[1][0] * c, elements[1][1] * c);
}
Mat22 Mat22::operator/(float &c) const
{
	return Mat22(elements[0][0] / c, elements[0][1] / c, elements[1][0] / c, elements[1][1] / c);
}