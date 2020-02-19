//#pragma once

#include <cmath>
#include "vec3.h"

namespace LilEngie::Math
{
	vec3::vec3()
		: x(0), y(0), z(0) { }

	vec3::vec3(float x, float y, float z)
		: x(x), y(y), z(z) { }

	vec3 vec3::operator+(const vec3& v)
	{
		return {
			x + v.x,
			y + v.y,
			z + v.z,
		};
	}

	void vec3::operator+=(const vec3& v)
	{
		x += v.x;
		y += v.y;
		z += v.z;
	}

	vec3 vec3::operator*(float f)
	{
		return { x * f, y * f, z * f };
	}

	vec3 vec3::operator/(float f)
	{
		return { x / f, y / f, z / f };
	}

	float& vec3::operator[](int i)
	{
		return s[i];
	}

	float vec3::operator[](int i) const
	{
		return s[i];
	}

	float dot(vec3 a, vec3 b)
	{
		return a.x * b.x + a.y * b.y + a.z * b.z;
	}

	vec3 cross(vec3 a, vec3 b)
	{
		return {
			a.y * b.z - a.z * b.y,
			a.z * b.x - a.x * b.z,
			a.x * b.y - a.y * b.x
		};
	}

	float len(vec3 v)
	{
		return sqrt(v.x * v.x + v.y * v.y + v.z * v.z);
	}

	float len2(vec3 v)
	{
		return v.x * v.x + v.y * v.y + v.z * v.z;
	}

	void normalize(vec3& v)
	{
		v =  v / len(v);
	}
}
