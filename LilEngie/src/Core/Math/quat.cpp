#include <sstream>
#include <iomanip>
#include "vec3.h"
#include "vec4.h"
#include "quat.h"

namespace LilEngie::Math
{
	quat::quat()
		: x(0), y(0), z(0), w(1) { }

	quat::quat(float x, float y, float z, float w)
		: x(x), y(y), z(z), w(w) { }

	quat::quat(vec3 v, float s)
		: v(v), s(s) { }

	quat::quat(vec4 v)
		: v4(v) { }

	quat quat::Euler(vec3 e)
	{
		float cx = cos(e.x / 2);
		float cy = cos(e.y / 2);
		float cz = cos(e.z / 2);

		float sx = sin(e.x / 2);
		float sy = sin(e.y / 2);
		float sz = sin(e.z / 2);

		return {
			(sx * cy * cz) + (cx * sy * sz),
			(cx * sy * cz) - (sx * cy * sz),
			(cx * cy * sz) + (sx * sy * cz),
			(cx * cy * cz) - (sx * sy * sz)
		};
	}

	vec3 quat::Euler() const
	{
		//Really needed help for this one
		//https://www.euclideanspace.com/maths/geometry/rotations/conversions/quaternionToEuler/Quaternions.pdf

		return {
			atan2(2*(w*x - y*z), 1 - 2*(x*x + y*y)),
			asin(2*(w*y + z*x)),
			atan2(2*(w*z - x*y), 1 - 2*(y*y + z*z)),
		};
	}

	quat quat::AxisAngle(vec3 a, float t)
	{
		return {
			a * sin(t / 2),
			cos(t / 2)
		};
	}

	vec4 quat::AxisAngle() const
	{
		//Avoid division by 0
		if (w == 1) return vec4();

		//Calculate axis angle parts
		return {
			v / sqrt(1 - (w * w)),
			2 * acos(w)
		};
	}

	//TODO
	//quat quat::RotationMatrix(const mat4& m)
	//{
	//	return quat();
	//}

	mat4 quat::RotationMatrix() const
	{
		mat4 m = mat4(1);

		//Rotate each individual column vector
		for (int i = 0; i < 3; i++)
			m[i].v3 = RotatePoint(m[i].v3);

		return m;
	}

	vec3 quat::RotatePoint(const vec3& pt) const
	{
		return (*this * quat(pt, 0) * Conjugate()).v;
	}

	void quat::Rotate(const quat& q)
	{
		*this = q * *this;
	}

	quat quat::Conjugate() const
	{
		return quat(-v, s);
	}

	quat quat::Inverse() const
	{
		return Conjugate() / SqrLength();
	}

	float quat::Length() const
	{
		return sqrt(x*x + y*y + z*z + w*w);
	}

	float quat::SqrLength() const
	{
		return x*x + y*y + z*z + w*w;
	}

	quat quat::operator*(const quat& q) const
	{
		return {
			(w * q.x) + (x * q.w) + (y * q.z) - (z * q.y),
			(w * q.y) - (x * q.z) + (y * q.w) + (z * q.x),
			(w * q.z) + (x * q.y) - (y * q.x) + (z * q.w),
			(w * q.w) - (x * q.x) - (y * q.y) - (z * q.z)
		};
	}

	void quat::operator*=(const quat& q)
	{
		*this = *this * q;
	}

	quat quat::operator*(float s) const
	{
		return v4 * s;
	}

	void quat::operator*=(float s)
	{
		v4 *= s;
	}

	quat quat::operator/(float s) const
	{
		return v4 / s;
	}

	void quat::operator/=(float s)
	{
		v4 /= s;
	}

	std::ostream& operator<<(std::ostream& os, const quat& q)
	{
		os << std::setprecision(3) << std::fixed;
		return os << "(" << q.w << " + " << q.x << "i + " << q.y << "j + " << q.z << "k)";
	}
}
