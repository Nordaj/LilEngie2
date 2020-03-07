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
		ICBuffer* buf = Renderer::core->cbPerObject;
		void* loc = gfx->GetCBufferPtr(buf);
		memcpy(loc, &transform, sizeof(mat4));
		gfx->UpdateCBuffer(buf);

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
