#pragma once

#include "vec3.h"
#include "mat4.h"

namespace LilEngie::Math
{
	//Returns a point on ray p(s) that is nearest to ray q(t)
	extern vec3 RayRayNearest(vec3 po, vec3 pd, vec3 qo, vec3 qd);

	extern float RayPointDistance(vec3 ro, vec3 rd, vec3 p);
	extern float LinePointDistance(vec3 a, vec3 b, vec3 pt);

	extern vec3 LineClosestPoint(vec3 a, vec3 b, vec3 p);

	//Returns t of ray where intersected, + = ro is in front
	extern float RayPlaneInt(vec3 ro, vec3 rd, vec3 po, vec3 pn);
	extern void ClipLinePlane(vec3& a, vec3& b, vec3 po, vec3 pn);

	extern void FrustrumRaycast(const mat4& vp, vec3 pt, vec3& ro, vec3& rd);
}
