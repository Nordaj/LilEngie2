#pragma once

#include <string>
#include <Core/Core.h>

namespace LilEngie
{
	enum class ResourceType
	{
		Mesh
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

		bool operator<(const ResourceId& other) const
		{
			return path < other.path;
		}
	};
}
