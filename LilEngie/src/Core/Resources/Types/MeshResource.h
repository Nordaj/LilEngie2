#pragma once

#include <string>
#include <vector>
#include <Core/Core.h>
#include <Core/Graphics/Mesh.h>
#include "../IResource.h"

namespace LilEngie
{
	class LIL_API MeshResource : public IResource
	{
	public:
		Mesh mesh;

	public:
		MeshResource(const ResourceId& resourceId, const ResourceManager* manager, bool load = true);
		~MeshResource();

		bool LoadMesh();
		bool IsLoaded();
	};
}
