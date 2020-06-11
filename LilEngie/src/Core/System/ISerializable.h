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

	protected:
		//Can handle both serialize and deserialize, up to derrived which to override
		virtual void Serialization(json& j, bool serialize)
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
			else
				LIL_WARN("Property: ", name, " is not serializable. Specialize SerializeProperty<T>() to add support.");
		}

		//Deserialization template (to be specialized)
		template<class T>
		void DeserializeProperty(json& j, T& data, std::string name)
		{
			if (std::is_base_of<ISerializable, T>())
				Deserialize(j[name]);
			else
				LIL_WARN("Property: ", name, " is not deserializable. Specialize DeserializeProperty<T>() to add support.");
		}
	};
}

#define PROPERTY(p) PropertySerialization(j, p, _STR(p), serialize);
#define NAMED_PROPERTY(p,n) PropertySerialization(j, p, n, serialize);

#define PROPERTIES(x) \
protected: \
void Serialization(json& j, bool serialize) override \
{ \
	x \
}

#include "SerializableSpecs.ipp"
