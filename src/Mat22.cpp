#include "Mat22.h"
#include "Vec2.h"

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

Mat22 Mat22::T() const
{
	return Mat22(elements[0][0], elements[1][0], elements[0][1], elements[1][1]);
}

float Mat22::norm() const
{
	float sqSum = elements[0][0] * elements[0][0]
		+ elements[0][1] * elements[0][1]
		+ elements[1][0] * elements[1][0]
		+ elements[1][1] * elements[1][1];
	return sqrtf(sqSum);
}

float Mat22::operator()(int row, int col) const { return elements[row][col]; }

std::ostream &operator<<(std::ostream &os, const Mat22 &m)
{
	os << "(" << m.elements[0][0] << ", " << m.elements[0][1] << ", " << m.elements[1][0] << ", " << m.elements[1][1] << ")";
	return os;
}

Mat22 Mat22::operator+(const Mat22 &mat) const
{
	float m[2][2] = {
		{elements[0][0] + mat(0, 0), elements[0][1] + mat(0, 1)},
		{elements[1][0] + mat(1, 0), elements[1][1] + mat(1, 1)} };
	return Mat22(m);
}
Mat22 Mat22::operator-(const Mat22 &mat) const
{
	float m[2][2] = {
		{elements[0][0] - mat(0, 0), elements[0][1] - mat(0, 1)},
		{elements[1][0] - mat(1, 0), elements[1][1] - mat(1, 1)} };
	return Mat22(m);
}
Mat22 Mat22::operator*(const Mat22 &mat) const
{
	float x1 = elements[0][0] * mat(0, 0) + elements[0][1] * mat(1, 0);
	float x2 = elements[0][0] * mat(0, 1) + elements[0][1] * mat(1, 1);
	float x3 = elements[1][0] * mat(0, 0) + elements[1][1] * mat(1, 0);
	float x4 = elements[1][0] * mat(0, 1) + elements[1][1] * mat(1, 1);
	return Mat22(x1, x2, x3, x4);
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
	return v * T();
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


Mat22 rotMat(float theta)
{
	return Mat22(cosf(theta), -sinf(theta), sinf(theta), cosf(theta));
}


Mat22 zoomMat(float rx, float ry)
{
	return Mat22(1.0f * rx, 0.0f, 0.0f, 1.0f * ry);
}