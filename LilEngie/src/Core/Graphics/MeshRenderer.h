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
	class Material;

	class LIL_API MeshRenderer : public IRenderable
	{
	private:
		MeshResource* meshResource;

	public:
		Material* material = nullptr;
		ResourceId meshResId;
		mat4 transform;

	public:
		MeshRenderer() {}
		void Render(IGraphics* gfxContext);

	private:
		MeshResource* GetMesh();
	};
}
