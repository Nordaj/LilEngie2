#pragma once

#include <string>
#include <unordered_map>
#include <Core/Debug/Log.h>
#include <Core/Core.h>

namespace LilEngie
{
	class IResource;

	enum class ResourceType
	{
		Mesh,
		Shader,
		Material,
		Texture
	};

	static std::unordered_map<std::string, ResourceType> resourceTypeNames = {
		{"mesh", ResourceType::Mesh},
		{"shader", ResourceType::Shader},
		{"material", ResourceType::Material},
		{"texture", ResourceType::Texture},
	};

	class LIL_API ResourceId
	{
	public:
		std::string path;
		ResourceType type;

	public:
		ResourceId() {}
		ResourceId(std::string& path, ResourceType type)
			: path(path), type(type) { }

		IResource* Get();

		bool operator<(const ResourceId& other) const
		{
			return path < other.path;
		}
	};
}
