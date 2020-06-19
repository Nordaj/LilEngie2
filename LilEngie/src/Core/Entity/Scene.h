#pragma once

#include <string>
#include <map>
#include <Core/Core.h>
#include <Core/System/ISerializable.h>
#include "Actor.h"

namespace LilEngie
{
	class LIL_API Scene : public ISerializable
	{
		friend class SceneManager;

	public:
		SceneManager* manager;
		std::string name;

	private:
		std::map<std::string, Actor*> actors;
		Actor* root;
		bool isEnabled;
		bool isInitialized;
		bool isCleaned;

	public:
		Scene();
		~Scene();

		void Init(bool enable = true);
		bool IsIntialized();
		void Clean();

		void SetEnabled(bool val);
		inline bool IsEnabled();

		void Start();
		void Update();
		void OnDraw();
		void OnDrawImGui();

		Actor* CreateActor(std::string uid, Actor* parent = nullptr);
		void DestroyActor(Actor* actor);

		Actor* GetActor(std::string uid);
		void SetActorParent(Actor* actor, Actor* parent);

		void Deserialize(json& j) override;
		void Serialize(json& j) override;
	};
}
