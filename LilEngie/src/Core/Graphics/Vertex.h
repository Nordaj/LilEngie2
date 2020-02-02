#pragma once

#include <Core/Core.h>

namespace LilEngie
{
	//temp
	struct LIL_API vec3
	{
		float x, y, z;
	};

	struct LIL_API Vertex
	{
		vec3 position;
		vec3 normal;
		vec3 tangent;
		float u, v;
	};
}
