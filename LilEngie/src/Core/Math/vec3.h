#pragma once

#include <Core/Core.h>

namespace LilEngie::Math
{
	struct LIL_API vec3
	{
		union
		{
			struct { float x, y, z; };

			struct { float r, g, b; };

			float s[3];
		};

		vec3();
		vec3(float x, float y, float z);

		vec3 operator+(const vec3& v);
		void operator+=(const vec3& v);

		vec3 operator*(float f);
		vec3 operator/(float f);

		float& operator[](int i);
		float operator[](int i) const;
	};

	extern float dot(vec3 a, vec3 b);
	extern vec3 cross(vec3 a, vec3 b);
	extern float len(vec3 vec);
	extern float len2(vec3 vec);
	extern void normalize(vec3& vec);
}
