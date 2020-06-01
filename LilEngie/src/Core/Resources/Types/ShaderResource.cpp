#include <string>
#include <Core/Platform/Graphics/IGraphics.h>
#include <Core/Platform/Graphics/Handles.h>
#include <Core/Graphics/Renderer.h>
#include "ShaderResource.h"

namespace LilEngie
{
	ShaderResource::ShaderResource(const ResourceId& resourceId, ResourceManager* manager, bool load, IGraphics* gfx)
		: IResource(resourceId, manager), shader(nullptr)
	{
		if (load)
			LoadShader();

		this->gfx = gfx ? gfx : Renderer::core->gfx;
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
