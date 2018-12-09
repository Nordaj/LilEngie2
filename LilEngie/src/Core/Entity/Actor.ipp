#pragma once

namespace LilEngie
{
	template<typename T>
	T* Actor::CreateComponent()
	{
		T* component = new T();
		components.push_back(component);
		return component;
	}
}
