#pragma once

#include <string>
#include <Core/Core.h>
#include "Actor.h"

namespace LilEngie
{
	class LIL_API Scene
	{
	public:
		std::string name;

	private:
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

		void Update();
		void OnDraw();

		Actor* CreateActor(Actor* parent = nullptr);
		void DestroyActor(Actor* actor);
	};
}
