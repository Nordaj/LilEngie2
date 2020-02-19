#include <Core/Graphics/Mesh.h>
#include "MeshComponent.h"

namespace LilEngie
{
	void MeshComponent::Update()
	{
		if (mesh)
			mesh->Render();
	}
}
