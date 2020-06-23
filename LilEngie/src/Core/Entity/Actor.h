#pragma once

#include <string>
#include <vector>
#include <Core/Core.h>
#include <Core/System/ISerializable.h>
#include "IComponent.h"

namespace LilEngie
{
	class TransformComponent;
	class Game;

	class LIL_API Actor : public ISerializable
	{
		friend class Scene;
		friend class SceneManager;

	public:
		std::string name;
		std::string uid;
		Actor* parent = nullptr;
		Scene* scene;
		Game* game;
		TransformComponent* transform = nullptr;

		bool editorOnly = false;
		bool hideInTree = false;
		bool serialize = true;

	private:
		std::vector<IComponent*> components;
		bool isEnabled = true;
		std::vector<Actor*> children;

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
		bool ContainsComponent(std::string id);
		int ComponentsCount();
		IComponent* GetComponent(int i);

		void Deserialize(json& j) override;
		void Serialize(json& j) override;

	private:
		void Start();
		void Update();
		void OnDraw();
		void OnDrawImGui();
	};
}

#include "Actor.ipp"
