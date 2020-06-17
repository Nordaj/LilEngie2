#include <string>
#include <Core/Platform/Graphics/IGraphics.h>
#include <Core/Platform/Graphics/Handles.h>
#include <Core/Graphics/Renderer.h>
#include <Core/Game/Game.h>
#include "../ResourceManager.h"
#include "ShaderResource.h"

namespace LilEngie
{
	ShaderResource::ShaderResource(const ResourceId& resourceId, ResourceManager* manager, bool load)
		: IResource(resourceId, manager), shader(nullptr)
	{
		if (load)
			LoadShader();

		gfx = manager->game->renderer.gfx;
	}

	ShaderResource::~ShaderResource()
	{
		gfx->ReleaseShader(&shader);
	}

	bool ShaderResource::LoadShader(std::string vert, std::string frag)
	{
		if (vert != "" && frag != "")
			shader = gfx->CreateShader(vert, frag);
		else
			shader = gfx->CreateShader(resourceId.path + "VS", resourceId.path + "FS");
		return IsLoaded();
	}

	bool ShaderResource::IsLoaded()
	{
		return shader;
	}
}
