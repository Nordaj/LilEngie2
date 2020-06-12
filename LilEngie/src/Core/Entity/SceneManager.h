#pragma once

#include <string>
#include <Core/Core.h>

namespace LilEngie
{
	class Scene;
	class Actor;
	class IComponent;

	typedef IComponent*(*ComponentFactoryFunc)(Actor*, std::string);

	class LIL_API SceneManager
	{
	public:
		static SceneManager* core;

		Scene* scene = nullptr;
		ComponentFactoryFunc gameComponentFactory;

	public:
		void UnloadScene();
		void Shutdown();

		bool LoadScene(const char* path);
		bool SaveScene(const char* path, Scene* scn = nullptr);
	};
}
