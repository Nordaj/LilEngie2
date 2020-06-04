#include <stb/stb_image.h>
#include <Core/Debug/Log.h>
#include <Core/Platform/Graphics/Handles.h>
#include <Core/Platform/Graphics/IGraphics.h>
#include <Core/Graphics/Renderer.h>
#include "TextureResource.h"

namespace LilEngie
{
	TextureResource::TextureResource(const ResourceId& resourceId, ResourceManager* manager, IGraphics* gfx)
		: IResource(resourceId, manager), texture(nullptr)
	{ 
		this->gfx = gfx ? gfx : Renderer::core->gfx;
	}

	TextureResource::~TextureResource()
	{
		gfx->ReleaseTexture(&texture);
	}

	bool TextureResource::LoadTexture()
	{
		//Load texture
		int w, h, ch;
		stbi_set_flip_vertically_on_load(true);
		uint8* image = stbi_load(resourceId.path.c_str(), &w, &h, &ch, STBI_rgb_alpha);

		//Create texture if i can
		if (image)
		{
			texture = gfx->CreateTexture(w, h, TextureFormat::R8G8B8A8, image);
			stbi_image_free(image);
		}
		else
			LIL_ERROR("Could not load texture data from file...");

		return IsLoaded();
	}

	bool TextureResource::IsLoaded()
	{
		return texture;
	}
}
