#include <Core/Graphics/Mesh.h>
#include <Core/Graphics/Renderer.h>
#include <Core/Graphics/MeshRenderer.h>
#include "MeshComponent.h"

namespace LilEngie
{
	void MeshComponent::Update()
	{
		
	}

	void MeshComponent::OnDraw()
	{
		//Manage constant buffer data
		rotate(meshRenderer.transform, vec3(0, 0, .01f));
		//translate(meshRenderer.transform, vec3(.0001f, 0, 0));

		//In future do cull check here TODO

		//Submit to render queue
		Renderer::core->QueueOpaque(&meshRenderer);
	}
}
