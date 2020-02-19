#pragma once

#include <Core/Core.h>

namespace LilEngie
{
	class Scene;

	class LIL_API SceneManager
	{
	public:
		static SceneManager* core;

		Scene* scene = nullptr;

	public:
		void Shutdown();
	};
}
