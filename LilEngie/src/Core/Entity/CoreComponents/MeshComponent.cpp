#include <Core/Graphics/Mesh.h>
#include <Core/Graphics/Renderer.h>
#include <Core/Graphics/MeshRenderer.h>
#include <Core/Debug/Log.h>
#include <Core/Resources/ResourceManager.h>
#include <Core/Resources/Types/MaterialResource.h>
#include "TransformComponent.h"
#include "../Actor.h"
#include "MeshComponent.h"

namespace LilEngie
{
	void MeshComponent::SetMaterial(Material* material)
	{
		meshRenderer.material = material;
	}

	void MeshComponent::Init()
	{
		meshRenderer.meshResId = meshId;

		MaterialResource* matRes = (MaterialResource*)matId.Get();
		if (!matRes) return;

		SetMaterial(matRes->material);
	}

	void MeshComponent::Update()
	{

	}

	void MeshComponent::OnDraw()
	{
		//Manage constant buffer data
		meshRenderer.transform = actor->transform->mat;

		//In future do cull check here TODO

		//Submit to render queue
		Renderer::core->QueueOpaque(&meshRenderer);
	}
}
