#include <cmath>
#include "vec3.h"
#include "vec4.h"

namespace LilEngie::Math
{
	vec4::vec4()
		: x(0), y(0), z(0), w(0) { }

	vec4::vec4(float x, float y, float z, float w)
		: x(x), y(y), z(z), w(w) { }

	vec4::vec4(vec3 v, float w)
		: x(v.x), y(v.y), z(v.z), w(w) { }

	vec4 vec4::operator+(const vec4& v) const
	{
		return {
			x + v.x,
			y + v.y,
			z + v.z,
			w + v.w
		};
	}

	void vec4::operator+=(const vec4& v)
	{
		x += v.x;
		y += v.y;
		z += v.z;
		w += v.w;
	}

	vec4 vec4::operator*(float f) const
	{
		return { x * f, y * f, z * f , w * f };
	}

	vec4 vec4::operator*=(float f)
	{
		return *this = *this * f;
	}

	vec4 vec4::operator/(float f) const
	{
		return { x / f, y / f, z / f, w * f };
	}

	vec4 vec4::operator/=(float f)
	{
		return *this = *this / f;
	}

	float& vec4::operator[](int i)
	{
		return s[i];
	}

	float vec4::operator[](int i) const
	{
		return s[i];
	}

	float dot(vec4 a, vec4 b)
	{
		return a.x * b.x + a.y * b.y + a.z * b.z + a.w * b.w;
	}

	float len(vec4 v)
	{
		return sqrt(v.x * v.x + v.y * v.y + v.z * v.z + v.w * v.w);
	}

	float len2(vec4 v)
	{
		return v.x * v.x + v.y * v.y + v.z * v.z + v.w * v.w;
	}

	void normalize(vec4& v)
	{
		v = v / len(v);
	}
}
