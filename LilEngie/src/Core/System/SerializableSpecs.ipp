#pragma once

namespace LilEngie
{
	//int
	template<>
	inline void ISerializable::SerializeProperty<int>(json& j, int& data, std::string name)
	{
		j[name] = data;
	}

	template<>
	inline void ISerializable::DeserializeProperty<int>(json& j, int& data, std::string name)
	{
		data = j[name].get<int>();
	}

	//bool
	template<>
	inline void ISerializable::SerializeProperty<bool>(json& j, bool& data, std::string name)
	{
		j[name] = data;
	}

	template<>
	inline void ISerializable::DeserializeProperty<bool>(json& j, bool& data, std::string name)
	{
		data = j[name].get<bool>();
	}

	//float
	template<>
	inline void ISerializable::SerializeProperty<float>(json& j, float& data, std::string name)
	{
		j[name] = data;
	}

	template<>
	inline void ISerializable::DeserializeProperty<float>(json& j, float& data, std::string name)
	{
		data = j[name].get<float>();
	}

	//std::string
	template<>
	inline void ISerializable::SerializeProperty<std::string>(json& j, std::string& data, std::string name)
	{
		j[name] = data;
	}

	template<>
	inline void ISerializable::DeserializeProperty<std::string>(json& j, std::string& data, std::string name)
	{
		data = j[name].get<std::string>();
	}

	//vec3
	template<>
	inline void ISerializable::SerializeProperty<vec3>(json& j, vec3& data, std::string name)
	{
		j[name] = { data.x, data.y, data.z };
	}

	template<>
	inline void ISerializable::DeserializeProperty<vec3>(json& j, vec3& data, std::string name)
	{
		data = vec3(j[name][0].get<float>(), j[name][1].get<float>(), j[name][2].get<float>());
	}

	//vec4
	template<>
	inline void ISerializable::SerializeProperty<vec4>(json& j, vec4& data, std::string name)
	{
		j[name] = { data.x, data.y, data.z, data.w };
	}

	template<>
	inline void ISerializable::DeserializeProperty<vec4>(json& j, vec4& data, std::string name)
	{
		data = vec4(j[name][0].get<float>(), j[name][1].get<float>(), j[name][2].get<float>(), j[name][3].get<float>());
	}

	//mat4
	template<>
	inline void ISerializable::SerializeProperty<mat4>(json& j, mat4& data, std::string name)
	{
		mat4 mat = transpose(data);
		j[name] = std::vector<float>(&mat.m[0], &mat.m[16]);
	}

	template<>
	inline void ISerializable::DeserializeProperty<mat4>(json& j, mat4& data, std::string name)
	{
		std::vector<float> m = j[name].get<std::vector<float>>();
		data = transpose(mat4(&m[0]));
	}

	//ResourceId
	template<>
	inline void ISerializable::SerializeProperty<ResourceId>(json& j, ResourceId& data, std::string name)
	{
		j[name].push_back(data.path);
		for (auto it = resourceTypeNames.begin(); it != resourceTypeNames.end(); it++)
		{
			if (it->second == data.type)
				j[name].push_back(it->first);
		}
	}

	template<>
	inline void ISerializable::DeserializeProperty<ResourceId>(json& j, ResourceId& data, std::string name)
	{
		data = ResourceId(j[name][0].get<std::string>(), resourceTypeNames[j[name][1].get<std::string>()]);
	}
}
