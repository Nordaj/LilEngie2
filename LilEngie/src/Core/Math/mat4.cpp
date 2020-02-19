#include <cmath>
#include "vec3.h"
#include "vec4.h"
#include "mat4.h"

namespace LilEngie::Math
{
	mat4::mat4()
		: mat4(1) { }

	mat4::mat4(float d)
	{
		c[0][0] = d;
		c[1][1] = d;
		c[2][2] = d;
		c[3][3] = d;
	}

	mat4::mat4(const float* e)
	{
		for (int i = 0; i < 16; i++)
			m[i] = e[i];
	}

	mat4::mat4(const vec4* col)
	{
		for (int i = 0; i < 4; i++)
			c[i] = col[i];
	}

	mat4::mat4(vec4 r0, vec4 r1, vec4 r2, vec4 r3)
	{
		c[0] = { r0.x, r1.x, r2.x, r3.x };
		c[1] = { r0.y, r1.y, r2.y, r3.y };
		c[2] = { r0.z, r1.z, r2.z, r3.z };
		c[3] = { r0.w, r1.w, r2.w, r3.w };
	}

	vec4 mat4::row(int i) const
	{
		return {
			c[0][i],
			c[1][i],
			c[2][i],
			c[3][i]
		};
	}

	vec4 mat4::operator*(const vec4& v) const
	{
		return mul(*this, v);
	}

	mat4 mat4::operator*(const mat4& m) const
	{
		return mul(*this, m);
	}

	vec4& mat4::operator[](int i)
	{
		return c[i];
	}

	vec4 mat4::operator[](int i) const
	{
		return c[i];
	}

	vec4 mul(const mat4& m, const vec4& v)
	{
		return {
			dot(m.row(0), v),
			dot(m.row(1), v),
			dot(m.row(2), v),
			dot(m.row(3), v)
		};
	}

	mat4 mul(const mat4& a, const mat4& b)
	{
		vec4 vecs[4] = {
			a * b[0],
			a * b[1],
			a * b[2],
			a * b[3]
		};
		return vecs;
	}

	void translate(mat4& m, vec3 v)
	{
		m[3].v3 += v;
	}

	void rotate(mat4& m, vec3 e)
	{
		//This function is slow as shit, will get to quaternions once i understand the dark magic
		float sx = sin(e.x);
		float cx = cos(e.x);

		float sy = sin(e.y);
		float cy = cos(e.y);

		float sz = sin(e.z);
		float cz = cos(e.z);

		mat4 rotX = mat4(
			{ 1., 0.,  0., 0. },
			{ 0., cx, -sx, 0. },
			{ 0., sx,  cx, 0. },
			{ 0., 0.,  0., 1. }
		);

		mat4 rotY = mat4(
			{  cy, 0., sy, 0. },
			{  0., 1., 0., 0. },
			{ -sy, 0., cy, 0. },
			{  0., 0., 0., 1. }
		);

		mat4 rotZ = mat4(
			{ cz, -sz, 0., 0. },
			{ sz,  cz, 0., 0. },
			{ 0.,  0., 0., 0. },
			{ 0.,  0., 0., 1. }
		);

		m = m * rotZ * rotY * rotX;
	}

	void scale(mat4& m, vec3 v)
	{
		m[0][0] *= v.x;
		m[1][1] *= v.y;
		m[2][2] *= v.z;
	}
}
