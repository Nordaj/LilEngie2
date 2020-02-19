#include <Core/Graphics/Mesh.h>
#include <Core/Graphics/Renderer.h>
#include "MeshComponent.h"

namespace LilEngie
{
	void MeshComponent::Update()
	{
		if (mesh)
			mesh->Render();
	}

	void MeshComponent::OnDraw()
	{
		//In future do cull check here TODO

		//Submit to render queue
		Renderer::core->QueueOpaque(mesh);
	}
}
