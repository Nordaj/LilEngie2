#pragma once

#include <string>
#include <Core/Core.h>

namespace LilEngie
{
	class Scene;
	class Actor;
	class Game;
	class IComponent;

	typedef IComponent*(*ComponentFactoryFunc)(Actor*, std::string);

	class LIL_API SceneManager
	{
	public:
		Game* game = nullptr;
		Scene* scene = nullptr;
		ComponentFactoryFunc gameComponentFactory = nullptr;

	public:
		void Init(Game* game);
		void Shutdown();

		bool LoadScene(const char* path);
		bool SaveScene(const char* path, Scene* scn = nullptr);
		void UnloadScene();
	};
}
