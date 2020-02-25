#pragma once

#include <Core/Core.h>

namespace LilEngie
{
	class IGraphics;

	class LIL_API IRenderable
	{
	public:
		IRenderable() {}
		virtual void Render(IGraphics* gfxContext) = 0;
	};
}
