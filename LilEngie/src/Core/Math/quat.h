#pragma once

#include <sstream>
#include <Core/Core.h>
#include "vec3.h"
#include "vec4.h"
#include "mat4.h"

namespace LilEngie::Math
{
	struct LIL_API quat
	{
		union
		{
			struct { float x, y, z, w; };

			struct { float i, j, k; };

			struct { float b, c, d, a; };

			struct { vec3 v; float s; };

			vec4 v4;

			float m[4];
		};

		quat();
		quat(float x, float y, float z, float w);
		quat(vec3 v, float s);
		quat(vec4 v);

		static quat Euler(vec3 euler);
		//vec3 Euler() const;

		static quat AxisAngle(vec3 axis, float angle);
		vec4 AxisAngle() const;

		//static quat RotationMatrix(const mat4& m);
		mat4 RotationMatrix() const;

		vec3 RotatePoint(const vec3& pt) const;

		void Rotate(const quat& q);

		quat Conjugate() const;
		quat Inverse() const;

		float Length() const;
		float SqrLength() const;

		quat operator*(const quat& q) const;
		void operator*=(const quat& q);

		quat operator*(float s) const;
		void operator*=(float s);

		quat operator/(float s) const;
		void operator/=(float s);
	};

	extern std::ostream& operator<<(std::ostream& os, const quat& q);
}
