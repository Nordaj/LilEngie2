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
	class Renderer;

	class LIL_API MeshRenderer : public IRenderable
	{
	private:
		MeshResource* meshResource = nullptr;

	public:
		Material* material = nullptr;
		ResourceId meshResId;
		mat4 transform;
		Renderer* renderer = nullptr;

	public:
		MeshRenderer() {}
		bool IsRenderable();
		void Init(Renderer* renderer);
		void Render(IGraphics* gfxContext) override;

	private:
		MeshResource* GetMesh();
	};
}
