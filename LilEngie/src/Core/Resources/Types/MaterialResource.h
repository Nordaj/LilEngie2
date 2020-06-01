#pragma once

#include <Core/Core.h>
#include <Core/Graphics/Material.h>
#include "../IResource.h"

namespace LilEngie
{
	class Material;
	class Renderer;
	class IShader;

	class LIL_API MaterialResource : public IResource
	{
	public:
		Material* material;

	public:
		MaterialResource(const ResourceId& resourceId, ResourceManager* manager, bool load = true);
		~MaterialResource();

		bool LoadMaterial();
		bool IsLoaded();
	};
}
