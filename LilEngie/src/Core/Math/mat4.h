#pragma once

#include <Core/Core.h>
#include "vec4.h"
#include "vec3.h"

namespace LilEngie::Math
{
	struct LIL_API mat4
	{
		union
		{
			float m[16] = { 0 };

			vec4 c[4];
		};

		mat4();
		mat4(float diag);
		mat4(const float* elements);
		mat4(const vec4* columns);
		mat4(vec4 r0, vec4 r1, vec4 r2, vec4 r3);

		vec4 row(int i) const;

		vec4 operator*(const vec4& v) const;
		mat4 operator*(const mat4& m) const;

		vec4& operator[](int i);
		vec4 operator[](int i) const;
	};

	extern vec4 mul(const mat4& m, const vec4& v);
	extern mat4 mul(const mat4& a, const mat4& b);

	extern void translate(mat4& m, vec3 v);
	extern void rotate(mat4& m, vec3 euler);
	extern void scale(mat4& m, vec3 v);
}
