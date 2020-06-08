#include <typeinfo>
#include <string>
#include <vector>
#include <nlohmann/json.hpp>
#include <Core/Math/LilMath.h>
#include <Core/Resources/ResourceId.h>
#include <Core/Debug/Log.h>
#include "ISerializable.h"

using json = nlohmann::json;

namespace LilEngie
{
	void ISerializable::SetProperties(json& j)
	{
		std::vector<SerializableProperty> properties = PropertyMap();
		for (SerializableProperty p : properties)
		{
			//Get value
			const char* tn = p.typeName.c_str();
			json val = j[p.name];
			if (val.is_null())
				continue;

			//Try to set property based on type
			if (!strcmp(tn, typeid(float).raw_name()))
				*((float*)p.value) = val.get<float>();
			else if (!strcmp(tn, typeid(int).raw_name()))
				*((int*)p.value) = val.get<int>();
			else if (!strcmp(tn, typeid(bool).raw_name()))
				*((bool*)p.value) = val.get<bool>();
			else if (!strcmp(tn, typeid(vec3).raw_name()))
			{
				vec3 v;
				for (int i = 0; i < 3; i++)
					v[i] = val[i].get<float>();

				*((vec3*)p.value) = v;
			}
			else if (!strcmp(tn, typeid(vec4).raw_name()))
			{
				vec4 v;
				for (int i = 0; i < 4; i++)
					v[i] = val[i].get<float>();

				*((vec4*)p.value) = v;
			}
			else if (!strcmp(tn, typeid(mat4).raw_name()))
			{
				float rowMajor[16] = {};
				for (int i = 0; i < 16; i++)
					rowMajor[i] = val[i].get<float>();
				mat4 m = transpose(mat4(rowMajor));
				*((mat4*)p.value) = m;
			}
			else if (!strcmp(tn, typeid(std::string).raw_name()))
				*((std::string*)p.value) = val.get<std::string>();
			else if (!strcmp(tn, typeid(ResourceId).raw_name()))
			{
				ResourceId resId;
				resId.path = val[0].get<std::string>();
				std::string type = val[1].get<std::string>();
				resId.type = resourceTypeNames[type];

				*((ResourceId*)p.value) = resId;
			}
			else if (!SetProperty(p, val))
			{
				if (p.isISerializable)
					((ISerializable*)p.value)->SetProperties(val);
				else
					LIL_WARN("Could not find a matching property type for property: ", p.name);
			}
		}
	}

	void ISerializable::SetJson(json& j)
	{
		//Set each property
		std::vector<SerializableProperty> properties = PropertyMap();
		for (SerializableProperty p : properties)
		{
			const char* tn = p.typeName.c_str();
			std::string name = p.name;

			if (!strcmp(tn, typeid(float).raw_name()))
				j[name] = *((float*)p.value);
			else if (!strcmp(tn, typeid(int).raw_name()))
				j[name] = *((int*)p.value);
			else if (!strcmp(tn, typeid(bool).raw_name()))
				j[name] = *((bool*)p.value);
			else if (!strcmp(tn, typeid(vec3).raw_name()))
			{
				vec3 v = *((vec3*)p.value);
				for (int i = 0; i < 3; i++)
					j[name].push_back(v[i]);
			}
			else if (!strcmp(tn, typeid(vec4).raw_name()))
			{
				vec4 v = *((vec4*)p.value);
				for (int i = 0; i < 4; i++)
					j[name].push_back(v[i]);
			}
			else if (!strcmp(tn, typeid(mat4).raw_name()))
			{
				mat4 m = transpose(*((mat4*)p.value));
				for (int i = 0; i < 16; i++)
					j[name].push_back(m.m[i]);
			}
			else if (!strcmp(tn, typeid(std::string).raw_name()))
				j[name] = *((std::string*)p.value);
			else if (!strcmp(tn, typeid(ResourceId).raw_name()))
			{
				ResourceId resId = *((ResourceId*)p.value);
				j[name].push_back(resId.path);

				for (auto it = resourceTypeNames.begin(); it != resourceTypeNames.end(); it++)
				{
					if (it->second == resId.type)
						j[name].push_back(it->first);
				}
			}
			else if (!SetJson(p, j))
			{
				if (p.isISerializable)
					((ISerializable*)p.value)->SetJson(j[p.name]);
				else
					LIL_WARN("Could not figure out how to save property: ", p.name);
			}
		}
	}
}
