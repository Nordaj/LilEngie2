#include <map>
#include <string>
#include <optional>
#include <Core/Debug/Log.h>
#include "ResourceManager.h"
#include "IResource.h"
#include "ResourceId.h"
#include "Types/MeshResource.h"

namespace LilEngie
{
	ResourceManager* ResourceManager::core = nullptr;

	ResourceManager::ResourceManager()
	{ }

	ResourceManager::~ResourceManager()
	{
		if (resources.size() > 0)
			UnloadAllResouces();
	}

	IResource* ResourceManager::LoadResource(const ResourceId& resourceId)
	{
		switch (resourceId.type)
		{
			case ResourceType::Mesh:
			{
				MeshResource* meshResource = new MeshResource(resourceId, this, false);
				if (!meshResource->LoadMesh())
				{
					LIL_ERROR("Could not load mesh resource...");
					return nullptr;
				}
				resources[resourceId] = meshResource;
				return meshResource;
			}
			default:
				LIL_WARN("Resource type: ", std::to_string((int)resourceId.type), " not supported. No resourced will be loaded.");
				return nullptr;
		}
	}

	IResource* ResourceManager::UnloadResource(const ResourceId& resourceId)
	{
		delete resources[resourceId];
		resources.erase(resourceId);
		return nullptr;
	}

	IResource* ResourceManager::GetResource(const ResourceId& resourceId)
	{
		return resources[resourceId];
	}

	void ResourceManager::UnloadAllResouces()
	{
		for (auto it = resources.begin(); it != resources.end(); it++)
			delete it->second;
		resources.clear();
	}
}
