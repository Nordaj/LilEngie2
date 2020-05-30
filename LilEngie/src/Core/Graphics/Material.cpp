#include <cstdlib>
#include <Core/Platform/Graphics/IGraphics.h>
#include <Core/Graphics/Renderer.h>
#include <Core/Debug/Log.h>
#include "Material.h"

namespace LilEngie
{
	Material::Material(IShader* shader, MaterialProperty properties[], uint n, Renderer* renderer)
		: shader(shader), propertyCount(n)
	{
		//Setup local buffer
		uint cbSize = 0;
		for (int i = 0; i < n; i++)
			cbSize += (uint)properties[i].type;

		//Setup material properties
		this->properties = new MaterialProperty[n];
		for (int i = 0; i < n; i++)
			this->properties[i] = properties[i];

		//Keep track of renderer
		ren = renderer == nullptr ? Renderer::core : renderer;

		//Setup cbuffer
		cBuffer = ren->gfx->CreateCBuffer(cbSize);
	}

	Material::~Material()
	{
		ren->gfx->ReleaseCBuffer(&cBuffer);
		delete[] properties;
	}

	bool Material::SetProperty(std::string name, const mat4& value)
	{
		//Find name match
		uint offset = 0;
		for (int i = 0; i < propertyCount; i++)
		{
			if (properties[i].name == name)
			{
				//Set data if type matches
				if (properties[i].type == MaterialPropertyType::MAT4)
				{
					char* p = (char*)ren->gfx->GetCBufferPtr(cBuffer);
					*((mat4*)(p + offset)) = value;
					return true;
				}
				else
					LIL_ERROR("Type mismatch, could not set material property: ", name);
			}
			offset += (uint)properties[i].type;
		}
		return false;
	}

	bool Material::SetProperty(std::string name, const vec4& value)
	{
		//Find name match
		for (int i = 0; i < propertyCount; i++)
		{
			uint offset = 0;
			if (properties[i].name == name)
			{
				//Set data if type matches
				if (properties[i].type == MaterialPropertyType::VEC4)
				{
					char* p = (char*)ren->gfx->GetCBufferPtr(cBuffer);
					*((vec4*)(p + offset)) = value;
					return true;
				}
				else
					LIL_ERROR("Type mismatch, could not set material property: ", name);
			}
			offset += (uint)properties[i].type;
		}
		return false;
	}

	bool Material::SetProperty(std::string name, const vec3& value)
	{
		//Find name match
		uint offset = 0;
		for (int i = 0; i < propertyCount; i++)
		{
			if (properties[i].name == name)
			{
				//Set data if type matches
				if (properties[i].type == MaterialPropertyType::VEC3)
				{
					char* p = (char*)ren->gfx->GetCBufferPtr(cBuffer);
					*((vec3*)(p + offset)) = value;
					return true;
				}
				else
					LIL_ERROR("Type mismatch, could not set material property: ", name);
			}
			offset += (uint)properties[i].type;
		}
		return false;
	}

	bool Material::SetProperty(std::string name, float value)
	{
		//Find name match
		uint offset = 0;
		for (int i = 0; i < propertyCount; i++)
		{
			if (properties[i].name == name)
			{
				//Set data if type matches
				if (properties[i].type == MaterialPropertyType::FLOAT)
				{
					char* p = (char*)ren->gfx->GetCBufferPtr(cBuffer);
					*((float*)(p + offset)) = value;
					return true;
				}
				else
					LIL_ERROR("Type mismatch, could not set material property: ", name);
			}
			offset += (uint)properties[i].type;
		}
		return false;
	}

	void Material::UpdateProperties()
	{
		ren->gfx->UpdateCBuffer(cBuffer);
	}

	void Material::BindCBuffer()
	{
		ren->gfx->BindCBuffer(cBuffer, ShaderType::Fragment, 3);
	}
}