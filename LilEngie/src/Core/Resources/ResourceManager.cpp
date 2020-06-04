#include <map>
#include <string>
#include <optional>
#include <Core/Debug/Log.h>
#include "IResource.h"
#include "ResourceId.h"
#include "Types/MeshResource.h"
#include "Types/ShaderResource.h"
#include "Types/MaterialResource.h"
#include "Types/TextureResource.h"
#include "ResourceManager.h"

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
		//Just return if resource is loaded
		IResource* res = GetResource(resourceId);
		if (res) return res;

		//Load resource
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
			case ResourceType::Shader:
			{
				ShaderResource* shaderResource = new ShaderResource(resourceId, this, false);
				if (!shaderResource->LoadShader())
				{
					LIL_ERROR("Could not load shader resource...");
					return nullptr;
				}
				resources[resourceId] = shaderResource;
				return shaderResource;
			}
			case ResourceType::Material:
			{
				MaterialResource* materialResource = new MaterialResource(resourceId, this, false);
				if (!materialResource->LoadMaterial())
				{
					LIL_ERROR("Could not load material resource...");
					return nullptr;
				}
				resources[resourceId] = materialResource;
				return materialResource;
			}
			case ResourceType::Texture:
			{
				TextureResource* textureResource = new TextureResource(resourceId, this, false);
				if (!textureResource->LoadTexture())
				{
					LIL_ERROR("Could not load texture resource...");
					return nullptr;
				}
				resources[resourceId] = textureResource;
				return textureResource;
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
		if (resources.find(resourceId) == resources.end())
			return nullptr;

		return resources[resourceId];
	}

	void ResourceManager::UnloadAllResouces()
	{
		for (auto it = resources.begin(); it != resources.end(); it++)
			delete it->second;
		resources.clear();
	}
}
