#pragma once

#include <string>
#include <Core/Core.h>
#include "../IResource.h"

namespace LilEngie
{
	class IShader;
	class IGraphics;

	class LIL_API ShaderResource : public IResource
	{
	public:
		IShader* shader;

	private:
		IGraphics* gfx;

	public:
		ShaderResource(const ResourceId& resourceId, ResourceManager* manager, bool load = true);
		~ShaderResource();

		//Shader creation from resource id path depends on the path not including the VS/FS types nor file extensions
		bool LoadShader(std::string vert = "", std::string frag = "");
		bool IsLoaded();
	};
}
