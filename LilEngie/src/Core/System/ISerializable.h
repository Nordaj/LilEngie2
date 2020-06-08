#pragma once

#include <typeinfo>
#include <string>
#include <vector>
#include <nlohmann/json.hpp>
#include <Core/Core.h>

namespace LilEngie
{
	struct SerializableProperty
	{
		std::string name;
		std::string typeName;
		void* value;
		bool isISerializable;
	};

	class LIL_API ISerializable
	{
	public:
		virtual std::vector<SerializableProperty> PropertyMap()
		{ return {}; }

		void SetProperties(nlohmann::json& j);
		virtual bool SetProperty(SerializableProperty property, nlohmann::json& value) 
		{ return false; }

		void SetJson(nlohmann::json& j);
		virtual bool SetJson(SerializableProperty property, nlohmann::json& j)
		{ return false; }
	};
}

#define PROPERTY(n) { _STR(n), typeid(n).raw_name(), &n, false },
#define PROPERTY(n, t) { _STR(n), typeid(n).raw_name(), &n, t },
#define PROPERTIES(p) \
std::vector<SerializableProperty> PropertyMap() override \
{ \
	return { p }; \
} \
