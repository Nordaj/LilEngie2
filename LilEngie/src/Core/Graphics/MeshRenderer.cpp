#include <Core/Platform/Graphics/IGraphics.h>
#include <Core/Resources/ResourceManager.h>
#include <Core/Resources/Types/MeshResource.h>
#include "Renderer.h"
#include "Mesh.h"
#include "Material.h"
#include "MeshRenderer.h"

namespace LilEngie
{
	void MeshRenderer::Init(Renderer* renderer)
	{
		this->renderer = renderer;
		GetMesh();
	}

	void MeshRenderer::Render(IGraphics* gfx)
	{
		//Skip if not ready
		if (!GetMesh())
			return;

		//Setup material if using it
		if (material)
		{
			gfx->SetShader(material->shader);
			material->BindMaterial();
		}

		//Update transform constant buffer
		ICBuffer* buf = renderer->cbPerObject;
		void* loc = gfx->GetCBufferPtr(buf);
		memcpy(loc, &transform, sizeof(mat4));
		gfx->UpdateCBuffer(buf);

		//Render
		meshResource->mesh.Render();
	}

	MeshResource* MeshRenderer::GetMesh()
	{
		if (!meshResource)
			meshResource = (MeshResource*)ResourceManager::core->LoadResource(meshResId);
		return meshResource;
	}
}
