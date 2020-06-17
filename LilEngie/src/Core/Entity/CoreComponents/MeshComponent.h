#pragma once

#include <Core/Graphics/MeshRenderer.h>
#include <Core/Entity/IComponent.h>
#include <Core/Core.h>

namespace LilEngie
{
	class Renderer;

	class LIL_API MeshComponent : public IComponent
	{
	public:
		ResourceId meshId;
		ResourceId matId;

		MeshRenderer meshRenderer;

		LIL_TYPE_NAME("mesh")
		PROPERTIES(
			PROPERTY(meshId)
			PROPERTY(matId)
		)

	private:
		Renderer* renderer;

	public:
		void SetMaterial(Material* material);

	private:
		void Init() override;

		//void Start() override;
		void Update() override;

		void OnDraw() override;
	};
}
