#include "IResource.h"
#include "ResourceManager.h"
#include "ResourceId.h"

namespace LilEngie
{
	IResource* ResourceId::Get()
	{
		IResource* res = ResourceManager::core->LoadResource(*this);
		if (!res)
			LIL_ERROR("Could not load resource.");
		return res;
	}
}
