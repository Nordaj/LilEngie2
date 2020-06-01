#pragma once

#include <vector>
#include <string>
#include <Core/Core.h>
#include "ResourceId.h"

namespace LilEngie
{
	class ResourceManager;

	class LIL_API IResource
	{
	public:
		const ResourceId resourceId;
		ResourceManager* manager;

	public:
		IResource(const ResourceId& resourceId, ResourceManager* manager)
			: resourceId(resourceId), manager(manager) { }

		virtual ~IResource() { }
	};
}
