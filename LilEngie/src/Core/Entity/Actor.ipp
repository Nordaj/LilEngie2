#pragma once

namespace LilEngie
{
	template<typename T>
	T* Actor::CreateComponent()
	{
		T* component = new T();
		component->actor = this;

		//Make sure the component has the other components it depends on
		if (!component->CheckDependencies())
		{
			delete component;
			return nullptr;
		}

		//Add to transform if its transform
		if (std::is_same<T, TransformComponent>::value)
			transform = (TransformComponent*)component;

		components.push_back(component);
		return component;
	}
}
