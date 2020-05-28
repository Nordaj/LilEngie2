#pragma once

#include <Core/Core.h>
#include "vec3.h"

namespace LilEngie::Math
{
	struct LIL_API vec4
	{
		union
		{
			struct { float x, y, z, w; };

			struct { float r, g, b, a; };

			vec3 v3;

			float s[4];
		};

		vec4();
		vec4(float x, float y, float z, float w);
		vec4(vec3 vec, float w = 0);

		vec4 operator+(const vec4& v) const;
		void operator+=(const vec4& v);

		vec4 operator*(float f) const;
		vec4 operator*=(float f);
		vec4 operator/(float f) const;
		vec4 operator/=(float f);

		float& operator[](int i);
		float operator[](int i) const;
	};

	extern float dot(vec4 a, vec4 b);
	extern float len(vec4 vec);
	extern float len2(vec4 vec);
	extern void normalize(vec4& vec);
}
