#pragma once

#include <string>
#include <Core/Core.h>

namespace LilEngie
{
	class Scene;
	class Actor;
	class Game;
	class IComponent;

	class LIL_API SceneManager
	{
	public:
		Game* game = nullptr;
		Scene* scene = nullptr;

	public:
		void Init(Game* game);
		void Shutdown();

		bool LoadScene(const char* path);
		bool SaveScene(const char* path = nullptr, Scene* scn = nullptr);
		void UnloadScene();
	};
}
