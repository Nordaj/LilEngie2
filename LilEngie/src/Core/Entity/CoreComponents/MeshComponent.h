#pragma once

#include <Core/Entity/IComponent.h>
#include <Core/Core.h>

namespace LilEngie
{
	class Mesh;

	class LIL_API MeshComponent : public IComponent
	{
	public:
		Mesh* mesh = nullptr;

	private:
		//void Start() override;
		void Update() override;
	};
}
