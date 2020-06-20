#include <string>
#include <fstream>
#include <sstream>
#include <array>
#include <nlohmann/json.hpp>
#include <Core/Math/LilMath.h>
#include <Core/Resources/ResourceId.h>
#include <Core/Resources/ResourceManager.h>
#include <Core/Game/Game.h>
#include "ComponentList.h"
#include "Scene.h"
#include "SceneManager.h"

using json = nlohmann::json;

namespace LilEngie
{
	std::vector<std::string> globalComponentIdList = {};

	void SceneManager::Init(Game* game)
	{
		this->game = game;

		//Push all component id's to global list
		CreateComponentFromString(nullptr, "__init");
	}

	void SceneManager::Shutdown()
	{
		UnloadScene();
	}

	bool SceneManager::LoadScene(const char* path)
	{
		//Unload current scene (uncomment later)
		UnloadScene();

		//Create scene object (use local scene later)
		Scene* scn = new Scene();
		scn->manager = this;
		scene = scn;
		scn->Init();

		//Get the actual path of the scene from resources relative path
		std::string realPath = path;
		realPath = game->resourceManager.GetResourcePath(realPath);

		//Get json string content of file
		std::string str;
		std::ifstream f;
		f.open(realPath);

		if (f.is_open())
		{
			std::stringstream s;
			s << f.rdbuf();
			str = s.str();
			f.close();
		}

		//Parse into json object TODO look into better parsing (log errors)
		json j = json::parse(str);

		//Deserialize
		scn->Deserialize(j);
		scn->Start();

		return true;
	}

	bool SceneManager::SaveScene(const char* path, Scene* scn)
	{
		//Make sure we have a scene
		if (!scn) scn = scene;
		if (!scene) return false;

		//Begin serialization
		json j;
		scene->Serialize(j);

		//Save json to file
		std::ofstream os(path, std::ofstream::trunc);
		os << j.dump(4);
		os.close();
	}

	void SceneManager::UnloadScene()
	{
		//In the future will also allocate scenes but for now we are only deallocating
		if (scene)
		{
			scene->Clean();
			delete scene;
		}
	}
}
