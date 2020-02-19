#pragma once

#include <Core/Core.h>
#include <Core/Math/LilMath.h>

namespace LilEngie
{
	struct LIL_API Vertex
	{
		vec3 position;
		vec3 normal;
		vec3 tangent;
		float u, v;
	};
}
