#pragma once

#include <string>
#include <map>
#include <optional>
#include <vector>
#include <Core/Core.h>
#include "IResource.h"
#include "ResourceId.h"

namespace LilEngie
{
	class Mesh;

	class LIL_API ResourceManager
	{
	public:
		static ResourceManager* core;

	private:
		std::map<ResourceId, IResource*> resources;

	public:
		ResourceManager();
		~ResourceManager();

		IResource* LoadResource(const ResourceId& resourceId);
		IResource* UnloadResource(const ResourceId& resourceId);

		//Returns nullptr if resource is not loaded
		IResource* GetResource(const ResourceId& resourceId);

		void UnloadAllResouces();
	};
}
