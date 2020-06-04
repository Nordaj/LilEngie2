#include <string>
#include <fstream>
#include <sstream>
#include <array>
#include <nlohmann/json.hpp>
#include <Core/Graphics/Material.h>
#include <Core/Graphics/Renderer.h>
#include <Core/Debug/Log.h>
#include <Core/Math/LilMath.h>
#include "ShaderResource.h"
#include "TextureResource.h"
#include "../ResourceManager.h"
#include "MaterialResource.h"

using json = nlohmann::json;

namespace LilEngie
{
	MaterialResource::MaterialResource(const ResourceId& resourceId, ResourceManager* manager, bool load)
		: IResource(resourceId, manager), material(nullptr)
	{ 
		if (load)
			LoadMaterial();
	}

	MaterialResource::~MaterialResource()
	{
		delete material;
	}

	bool MaterialResource::LoadMaterial()
	{
		//Get json string content of file
		std::string str;

		std::ifstream f;
		f.open(resourceId.path);

		if (f.is_open())
		{
			std::stringstream s;
			s << f.rdbuf();
			str = s.str();
			f.close();
		}

		//Parse into json object TODO look into better parsing (log errors)
		json obj = json::parse(str);

		//Get shader
		std::string shaderPath = obj["shader"];
		ResourceId shaderId = ResourceId(shaderPath, ResourceType::Shader);
		ShaderResource* shaderResource = (ShaderResource*)manager->LoadResource(shaderId);
		if (!shaderResource)
		{
			LIL_ERROR("Failed to load material shader resource: " + shaderPath);
			return false;
		}
		IShader* shader = shaderResource->shader;

		//Get material propeties
		std::vector<MaterialProperty> properties;
		for (auto& e : obj["properties"])
		{
			if (e["type"] == "float")
				properties.push_back({ e["name"], MaterialPropertyType::FLOAT });
			else if (e["type"] == "vec3")
				properties.push_back({ e["name"], MaterialPropertyType::VEC3 });
			else if (e["type"] == "vec4")
				properties.push_back({ e["name"], MaterialPropertyType::VEC4 });
			else if (e["type"] == "mat4")
				properties.push_back({ e["name"], MaterialPropertyType::MAT4 });
		}

		//Get textures
		std::vector<TextureProperty> texProperties;
		for (auto& e : obj["textures"])
		{
			//Get texture resource
			std::string path = e["path"];
			ResourceId texId = ResourceId(path, ResourceType::Texture);
			TextureResource* texResource = (TextureResource*)manager->LoadResource(texId);

			//Add to texture properties
			texProperties.push_back({ e["name"], texResource->texture, e["slot"] });
		}

		//Create material
		material = new Material(shader, &properties[0], properties.size(), &texProperties[0], texProperties.size());

		//Set property default values
		for (auto& e : obj["properties"])
		{
			if (e["type"] == "float")
				material->SetProperty(e["name"], e["value"]);
			else if (e["type"] == "vec3")
				material->SetProperty(e["name"], vec3(e["value"][0], e["value"][1], e["value"][2]));
			else if (e["type"] == "vec4")
				material->SetProperty(e["name"], vec4(e["value"][0], e["value"][1], e["value"][2], e["value"][2]));
			else if (e["type"] == "mat4")
			{
				mat4 m = mat4(
					{ e["value"][0], e["value"][1], e["value"][2], e["value"][3] },
					{ e["value"][4], e["value"][5], e["value"][6], e["value"][7] },
					{ e["value"][8], e["value"][9], e["value"][10], e["value"][11] },
					{ e["value"][12], e["value"][13], e["value"][14], e["value"][15] }
				);
				material->SetProperty(e["name"], m);
			}
		}
		material->UpdateProperties();

		return true;
	}

	bool MaterialResource::IsLoaded()
	{
		return material;
	}
}
