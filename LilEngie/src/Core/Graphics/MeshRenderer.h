#pragma once

#include <Core/Core.h>
#include <Core/Math/LilMath.h>
#include <Core/Resources/ResourceId.h>
#include "IRenderable.h"

namespace LilEngie
{
	class IGraphics;
	class ICBuffer;
	class MeshResource;

	class LIL_API MeshRenderer : public IRenderable
	{
	private:
		MeshResource* meshResource;

	public:
		ResourceId meshResId;
		mat4 transform;

	public:
		MeshRenderer();
		void Render(IGraphics* gfxContext);

	private:
		MeshResource* GetMesh();
	};
}
