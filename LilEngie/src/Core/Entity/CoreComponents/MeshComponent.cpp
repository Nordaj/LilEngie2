#include <Core/Graphics/Mesh.h>
#include <Core/Graphics/Renderer.h>
#include <Core/Graphics/MeshRenderer.h>
#include <Core/Debug/Log.h>
#include <Core/Resources/ResourceManager.h>
#include <Core/Resources/Types/MaterialResource.h>
#include "TransformComponent.h"
#include <Core/Game/Game.h>
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
		renderer = &actor->game->renderer;

		meshRenderer.meshResId = meshId;
		meshRenderer.Init(renderer);

		MaterialResource* matRes = (MaterialResource*)matId.Get();
		if (!matRes) return;

		SetMaterial(matRes->material);
	}

	void MeshComponent::Update()
	{

	}

	void MeshComponent::OnDraw()
	{
		if (!meshRenderer.IsRenderable())
			return;

		//Manage constant buffer data
		meshRenderer.transform = actor->transform->GlobalTransformation();

		//In future do cull check here TODO

		//Submit to render queue
		renderer->QueueOpaque(&meshRenderer);
	}
}
