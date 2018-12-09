#pragma once

#include <vector>
#include <Core/Core.h>
#include "Component.h"

namespace LilEngie
{
	class LIL_API Actor
	{
		friend class Scene;

	public:
		Actor* parent;

	private:
		Scene* scene;
		bool isEnabled = true;
		std::vector<Actor*> children;
		std::vector<Component*> components;

	public:
		Actor(Scene* scene);
		~Actor();

		int GetChildrenCount();
		Actor* GetChild(int index = 0);

		void SetEnabled(bool val);
		bool IsEnabled();

		void Destroy();

		void DestroyComponent(Component* component);
		template<typename T>
		T* CreateComponent();

	private:
		void Update();
	};
}

#include "Actor.ipp"
