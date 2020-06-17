#pragma once

#include <string>
#include <Core/Math/LilMath.h>
#include <Core/Core.h>

namespace LilEngie
{
	class IShader;
	class ICBuffer;
	class ITexture;
	class Renderer;

	enum class MaterialPropertyType : uint { MAT4 = 64, VEC4 = 16, VEC3 = 12, FLOAT = 4 };

	struct MaterialProperty
	{
		std::string name;
		MaterialPropertyType type;
	};

	struct TextureProperty
	{
		std::string name;
		ITexture* texture;
		uint slot;
	};

	class LIL_API Material
	{
	public:
		Renderer* ren;
		IShader* shader;
		ICBuffer* cBuffer;
		MaterialProperty* properties;
		uint propertyCount;
		TextureProperty* textures;
		uint textureCount;

	public:
		Material(IShader* shader, MaterialProperty properties[], uint propertyCount, TextureProperty textures[], uint textureCount, Renderer* renderer);
		~Material();

		bool SetProperty(std::string name, const mat4& value);
		bool SetProperty(std::string name, const vec4& value);
		bool SetProperty(std::string name, const vec3& value);
		bool SetProperty(std::string name, float value);
		void UpdateProperties();

		bool SetTexture(const std::string& name, TextureProperty texture);
		TextureProperty GetTexture(const std::string& name);

		void BindMaterial();
	};
}
