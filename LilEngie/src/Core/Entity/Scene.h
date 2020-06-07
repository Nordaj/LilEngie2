#pragma once

#include <string>
#include <map>
#include <Core/Core.h>
#include "Actor.h"

namespace LilEngie
{
	class LIL_API Scene
	{
	public:
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

		Actor* CreateActor(std::string uid, Actor* parent = nullptr);
		void DestroyActor(Actor* actor);

		Actor* GetActor(std::string uid);
	};
}
