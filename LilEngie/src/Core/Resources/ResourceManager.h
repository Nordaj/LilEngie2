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
	class Game;

	class LIL_API ResourceManager
	{
	public:
		static ResourceManager* core;
		Game* game;

	private:
		std::map<ResourceId, IResource*> resources;

	public:
		ResourceManager();
		~ResourceManager();

		void Init(Game* game);

		IResource* LoadResource(const ResourceId& resourceId);
		IResource* UnloadResource(const ResourceId& resourceId);

		//Returns nullptr if resource is not loaded
		IResource* GetResource(const ResourceId& resourceId);

		std::string GetResourcePath(const ResourceId& resourceId);
		std::string GetResourcePath(std::string& relPath);

		void UnloadAllResouces();
	};
}
