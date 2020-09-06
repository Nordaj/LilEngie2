#include <cmath>
#include <ostream>
#include <iomanip>
#include <sstream>
#include <Core/Debug/Log.h>
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

	mat4 translate(vec3 v)
	{
		mat4 m = mat4(1.);
		m[3].v3 += v;
		return m;
	}

	void translate(mat4& m, vec3 v)
	{
		m = translate(v) * m;
	}

	mat4 rotate(vec3 e)
	{
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
			{ cy, 0., sy, 0. },
			{ 0., 1., 0., 0. },
			{ -sy, 0., cy, 0. },
			{ 0., 0., 0., 1. }
		);

		mat4 rotZ = mat4(
			{ cz, -sz, 0., 0. },
			{ sz,  cz, 0., 0. },
			{ 0.,  0., 1., 0. },
			{ 0.,  0., 0., 1. }
		);

		return rotX * rotY * rotZ;
	}

	void rotate(mat4& m, vec3 e)
	{
		m = rotate(e) * m;
	}

	mat4 rotate(vec3 axis, float angle)
	{
		float c = cos(angle);
		float s = sin(angle);
		float t = 1 - c;
		vec3 a = axis / len(axis);

		//https://www.euclideanspace.com/maths/geometry/rotations/conversions/angleToMatrix/
		return mat4(
			{t * a.x * a.x + c,			t * a.x * a.y - a.z * s,	t * a.x * a.z + a.y * s,	0},
			{t * a.x * a.y + a.z * s,	t * a.y * a.y + c,			t * a.y * a.z - a.x * s,	0},
			{t * a.x * a.z - a.y * s,	t * a.y * a.z + a.x * s,	t * a.z * a.z + c,			0},
			{0,							0,							0,							1}
		);
	}

	void rotate(mat4& m, vec3 axis, float angle)
	{
		m = rotate(axis, angle) * m;
	}

	mat4 scale(vec3 v)
	{
		mat4 m = mat4(1.);
		m[0][0] *= v.x;
		m[1][1] *= v.y;
		m[2][2] *= v.z;
		return m;
	}

	void scale(mat4& m, vec3 v)
	{
		m = scale(v) * m;
	}

	mat4 projection(float r, float l, float t, float b, float n, float f)
	{
		//Left handed perspective matrix
		return mat4(
			{ (2*n)/(r-l),	0.,				(r+l)/(r-l),	0. },
			{ 0.,			(2*n)/(t-b),	(t+b)/(t-b),	0. },
			{ 0.,			0.,				(f+n)/(f-n),	-(2*f*n)/(f-n) },
			{ 0.,			0.,				1.,				0. }
		);
	}

	//Column major
	float det3x3(float m[9])
	{
		//Goes through top row
		return	(m[0] * ((m[4] * m[8]) - (m[7] * m[5]))) - 
				(m[3] * ((m[1] * m[8]) - (m[7] * m[2]))) + 
				(m[6] * ((m[1] * m[5]) - (m[2] * m[4])));
	}

	float determinant(const mat4& m)
	{
		//Goes through top row
		float c0[9] = { m[1][1], m[1][2], m[1][3], m[2][1], m[2][2], m[2][3], m[3][1], m[3][2], m[3][3] };
		float c1[9] = { m[0][1], m[0][2], m[0][3], m[2][1], m[2][2], m[2][3], m[3][1], m[3][2], m[3][3] };
		float c2[9] = { m[0][1], m[0][2], m[0][3], m[1][1], m[1][2], m[1][3], m[3][1], m[3][2], m[3][3] };
		float c3[9] = { m[0][1], m[0][2], m[0][3], m[1][1], m[1][2], m[1][3], m[2][1], m[2][2], m[2][3] };
		return	(m[0][0] * det3x3(c0)) - (m[1][0] * det3x3(c1)) + (m[2][0] * det3x3(c2)) - (m[3][0] * det3x3(c3));
	}

	mat4 inverse(const mat4& m)
	{
		//Get determinant
		float d = determinant(m);
		if (d == 0) return mat4(0.);

		//Get adjoint matrix
		mat4 adj = mat4();
		for (int e = 0; e < 16; e++)
		{
			//Get current element value (det of all other rows/cols)
			int current = 0;
			float others[9] = { 0 };
			for (int i = 0; i < 16; i++)
			{
				if (i / 4 != e / 4 && i % 4 != e % 4) 
					others[current++] = m.m[i];
			}

			adj[e % 4][e / 4] = det3x3(others) * ((e / 4 + e) % 2 == 0 ? 1 : -1);
		}

		//Return inv
		return (1 / d) * adj;
	}

	mat4 transpose(const mat4& m)
	{
		//I think this works idk
		mat4 mt;
		for (int i = 0; i < 16; i++)
			mt[i % 4][i / 4] = m.m[i];
		return mt;
	}

	mat4 operator*(float f, mat4 m)
	{
		for (int i = 0; i < 4; i++)
			m[i] *= f;
		return m;
	}

	std::ostream& operator<<(std::ostream& os, const mat4& m)
	{
		os << "\n" << std::setprecision(3);
		for (int i = 0; i < 4; i++)
		{
			for (int j = 0; j < 4; j++)
				os << "\t" << m[j][i];
			os << "\n";
		}
		return os;
	}
}
