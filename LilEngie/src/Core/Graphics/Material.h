#pragma once

#include <string>
#include <Core/Math/LilMath.h>
#include <Core/Core.h>

namespace LilEngie
{
	class IShader;
	class ICBuffer;
	class Renderer;

	enum class MaterialPropertyType : uint { MAT4 = 64, VEC4 = 16, VEC3 = 12, FLOAT = 4 };

	struct MaterialProperty
	{
		std::string name;
		MaterialPropertyType type;
	};

	class LIL_API Material
	{
	public:
		Renderer* ren;
		IShader* shader;
		ICBuffer* cBuffer;
		MaterialProperty* properties;
		uint propertyCount;

	public:
		Material(IShader* shader, MaterialProperty properties[], uint propertyCount, Renderer* renderer = nullptr);
		~Material();

		bool SetProperty(std::string name, const mat4& value);
		bool SetProperty(std::string name, const vec4& value);
		bool SetProperty(std::string name, const vec3& value);
		bool SetProperty(std::string name, float value);
		void UpdateProperties();

		void BindCBuffer();
	};
}
