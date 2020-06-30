#pragma once

#include <Core/Core.h>
#include <Core/Math/LilMath.h>

namespace LilEngie
{
	class IFramebuffer;

	class LIL_API Camera
	{
	public:
		IFramebuffer* framebuffer = nullptr;
		vec4 clearColor = vec4(0, 0, 0, 1);
		mat4 view = mat4(1);
		mat4 projection = mat4(1);
		bool renderDebug = false;
		float near = .1f;
	};
}
