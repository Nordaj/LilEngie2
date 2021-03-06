#include <map>
#include <string>
#include <optional>
#include <Core/Debug/Log.h>
#include <Core/Game/Game.h>
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

	void ResourceManager::Init(Game* game)
	{
		this->game = game;
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
					delete meshResource;
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
					delete shaderResource;
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
					delete materialResource;
					LIL_ERROR("Could not load material resource...");
					return nullptr;
				}
				resources[resourceId] = materialResource;
				return materialResource;
			}
			case ResourceType::Texture:
			{
				TextureResource* textureResource = new TextureResource(resourceId, this);
				if (!textureResource->LoadTexture())
				{
					delete textureResource;
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

	std::string ResourceManager::GetResourcePath(const ResourceId& resourceId)
	{
		return game->gamePath + resourceId.path;
	}

	std::string ResourceManager::GetResourcePath(std::string& relPath)
	{
		return game->gamePath + relPath;
	}

	void ResourceManager::UnloadAllResouces()
	{
		for (auto it = resources.begin(); it != resources.end(); it++)
			delete it->second;
		resources.clear();
	}
}
