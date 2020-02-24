#pragma once

#include <Core/Math/LilMath.h>
#include <Core/Core.h>
#include "IRenderable.h"

namespace LilEngie
{
	class IGraphics;
	class Mesh;
	class ICBuffer;

	class LIL_API MeshRenderer : public IRenderable
	{
	public:
		Mesh* mesh;
		mat4 transform;

	public:
		void Render(IGraphics* gfxContext);
	};
}
