#include <Core/Platform/Graphics/IGraphics.h>
#include <Core/Resources/ResourceManager.h>
#include <Core/Resources/Types/MeshResource.h>
#include "Renderer.h"
#include "Mesh.h"
#include "MeshRenderer.h"

namespace LilEngie
{
	MeshRenderer::MeshRenderer()
	{
		GetMesh();
	}

	void MeshRenderer::Render(IGraphics* gfx)
	{
		//Skip if not ready
		if (!GetMesh())
			return;

		//Update transform constant buffer
		ICBuffer* cb = Renderer::core->transformBuffer;
		memcpy(gfx->GetCBufferPtr(cb), &transform, sizeof(mat4));
		gfx->UpdateCBuffer(cb);

		//Render
		meshResource->mesh.Render(gfx);
	}

	MeshResource* MeshRenderer::GetMesh()
	{
		if (!meshResource)
			meshResource = (MeshResource*)ResourceManager::core->GetResource(meshResId);
		return meshResource;
	}
}
