#pragma once

#include <Core/Core.h>
#include <Core/Math/LilMath.h>

namespace LilEngie
{
	struct LIL_API Vertex
	{
		vec4 position;
		vec4 normal;
		vec4 tangent;
		float u, v;
		float padding[2];
	};
}
