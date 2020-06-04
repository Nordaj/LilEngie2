#pragma once

#include <Core/Core.h>
#include "../IResource.h"

namespace LilEngie
{
	class ITexture;
	class IGraphics;

	class LIL_API TextureResource : public IResource
	{
	public:
		ITexture* texture;

	private:
		IGraphics* gfx;

	public:
		TextureResource(const ResourceId& resourceId, ResourceManager* manager, IGraphics* gfx = nullptr);
		~TextureResource();

		bool LoadTexture();
		bool IsLoaded();
	};
}
