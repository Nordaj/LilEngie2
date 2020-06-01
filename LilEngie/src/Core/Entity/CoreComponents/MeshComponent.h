#pragma once

#include <Core/Graphics/MeshRenderer.h>
#include <Core/Entity/IComponent.h>
#include <Core/Core.h>

namespace LilEngie
{
	class MeshRenderer;

	class LIL_API MeshComponent : public IComponent
	{
	public:
		MeshRenderer meshRenderer;

	public:
		void SetMaterial(Material* material);

	private:
		//void Start() override;
		void Update() override;

		void OnDraw() override;
	};
}
