#pragma once

#include <string>
#include <vector>
#include <Core/Core.h>
#include "IComponent.h"

namespace LilEngie
{
	class TransformComponent;

	class LIL_API Actor
	{
		friend class Scene;
		friend class SceneManager;

	public:
		std::string name;
		std::string uid;
		Actor* parent;
		TransformComponent* transform;

	private:
		Scene* scene;
		bool isEnabled = true;
		std::vector<Actor*> children;
		std::vector<IComponent*> components;

	public:
		Actor(Scene* scene);
		~Actor();

		int GetChildrenCount();
		Actor* GetChild(int index = 0);

		void SetEnabled(bool val);
		bool IsEnabled();

		void Destroy();

		void DestroyComponent(IComponent* component);
		template<typename T>
		T* CreateComponent();

	private:
		void Start();
		void Update();
		void OnDraw();
	};

	class LIL_API ActorRef : public ISerializable
	{
	public:
		std::string uid;
		Actor* actor;

		PROPERTIES(
			PROPERTY(uid)
		)

	public:
		Actor* Get();

		Actor* operator->();
		operator Actor*() { return Get(); }
	};
}

#include "Actor.ipp"
