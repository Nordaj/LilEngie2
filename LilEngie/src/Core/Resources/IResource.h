#pragma once

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
		const ResourceManager* manager;

	public:
		IResource(const ResourceId& resourceId, const ResourceManager* manager)
			: resourceId(resourceId), manager(manager) { }

		virtual ~IResource() { }
	};
}
