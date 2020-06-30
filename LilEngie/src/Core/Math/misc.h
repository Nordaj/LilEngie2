#pragma once

#include "vec3.h"

namespace LilEngie::Math
{
	//Returns t of ray where intersected, + = ro is in front
	extern float RayPlaneInt(vec3 ro, vec3 rd, vec3 po, vec3 pn);
	extern void ClipLinePlane(vec3& a, vec3& b, vec3 po, vec3 pn);
}
