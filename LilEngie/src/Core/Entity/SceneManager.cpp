#include "Scene.h"
#include "SceneManager.h"

namespace LilEngie
{
	SceneManager* SceneManager::core = nullptr;

	void SceneManager::Shutdown()
	{
		scene->Clean();

		//In the future will also allocate scenes but for now we are only deallocating
		if (scene == nullptr)
			delete scene;
	}
}
