#pragma once

#include <typeinfo>
#include <string>
#include <vector>
#include <type_traits>
#include <nlohmann/json.hpp>
#include <Core/Debug/Log.h>
#include <Core/Core.h>
#include <Core/Math/LilMath.h>
#include <Core/Resources/ResourceId.h>

namespace LilEngie
{
	using json = nlohmann::json;

	class LIL_API ISerializable
	{
	public:
		virtual void Serialize(json& j)
		{ Serialization(j, true); }

		virtual void Deserialize(json& j)
		{ Serialization(j, false); }

		//Singular property setting/getting by name
		template <class T>
		void SetProperty(std::string name, T value)
		{
			//Creating a json with property name to set
			json j;
			j["propName"] = name;

			//Serializing it to the json
			SerializeProperty<T>(j, value, name);

			//Serialize it normally
			Serialization(j, false, true);
		}

		template <class T>
		T GetProperty(std::string name)
		{
			//Creating a json with property name to set
			json j;
			j["propName"] = name;

			//Serialize it normally
			Serialization(j, true, true);
		}

	protected:
		//Can handle both serialize and deserialize, up to derrived which to override
		virtual void Serialization(json& j, bool serialize, bool single = false)
		{ LIL_ERROR("No serialization handled for ISerializable class..."); }

		//Disperses serialization
		template<class T>
		void PropertySerialization(json& j, T& data, std::string name, bool serialize)
		{
			serialize ? 
				SerializeProperty(j, data, name) :
				DeserializeProperty(j, data, name);
		}

		//Serialization template (to be specialized)
		template<class T>
		void SerializeProperty(json& j, T& data, std::string name)
		{
			if (std::is_base_of<ISerializable, T>())
				Serialize(j[name]);
		}

		//Deserialization template (to be specialized)
		template<class T>
		void DeserializeProperty(json& j, T& data, std::string name)
		{
			if (std::is_base_of<ISerializable, T>())
				Deserialize(j[name]);
		}
	};
}

#define PROPERTY(p) \
if (!single || j["propName"] == _STR(p)) \
{PropertySerialization(j, p, _STR(p), serialize);} \

#define NAMED_PROPERTY(p,n) \
if (!single || j["propName"] == n) \
{PropertySerialization(j, p, n, serialize);} \

#define PROPERTIES(x) \
protected: \
void Serialization(json& j, bool serialize, bool single = false) override \
{ \
	x \
}

#include "SerializableSpecs.ipp"
