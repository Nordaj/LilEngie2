#include <string>
#include <fstream>
#include <sstream>
#include <array>
#include <nlohmann/json.hpp>
#include <Core/Math/LilMath.h>
#include <Core/Resources/ResourceId.h>
#include "ComponentList.h"
#include "Scene.h"
#include "SceneManager.h"

using json = nlohmann::json;

namespace LilEngie
{
	bool SetProperty(SerializableProperty& property, json& j);

	SceneManager* SceneManager::core = nullptr;

	void SceneManager::UnloadScene()
	{
		//In the future will also allocate scenes but for now we are only deallocating
		if (scene)
		{
			scene->Clean();
			delete scene;
		}
	}

	void SceneManager::Shutdown()
	{
		UnloadScene();
	}

	bool SceneManager::LoadScene(const char* path)
	{
		//Unload current scene (uncomment later)
		//UnloadScene();

		//Create scene object (use local scene later)
		Scene* scn = new Scene();
		scene = scn;
		scn->Init();

		//Get json string content of file
		std::string str;

		std::ifstream f;
		f.open(path);

		if (f.is_open())
		{
			std::stringstream s;
			s << f.rdbuf();
			str = s.str();
			f.close();
		}

		//Parse into json object TODO look into better parsing (log errors)
		json j = json::parse(str);

		//Loop through to create every actor initially
		for (auto& a : j["actors"])
		{
			//Create actor with name
			Actor* parent = nullptr;
			if (a["parent"] != "ROOT")
			{
				parent = scn->GetActor(a["parent"].get<std::string>());
				if (!parent) LIL_ERROR("Could not find actor parent: ", a["parent"].get<std::string>());
			}

			Actor* actor = scn->CreateActor(a["uid"].get<std::string>());
			actor->name = a["name"].get<std::string>();
		}

		//Create each component second
		for (auto& a : j["actors"])
		{
			//Get actor ptr
			Actor* actor = scn->GetActor(a["uid"].get<std::string>());
			if (!actor)
				LIL_ERROR("Could not find actor of uid, this is not supposed to happen.");

			//Create component
			for (auto& c : a["components"])
			{
				std::string type = c["type"].get<std::string>();
				IComponent* comp = CreateComponent(actor, type.c_str());

				//Make sure transform is added
				if (type == "transform")
					actor->transform = (TransformComponent*)comp;

				//Handle all properties deserialization
				comp->SetProperties(c["properties"]);

				comp->Init();
			}
		}

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
		for (auto it = scn->actors.begin(); it != scn->actors.end(); it++)
		{
			Actor* a = it->second;

			//Setup actor obj
			json actorJson;
			actorJson["name"] = a->name;
			actorJson["uid"] = a->uid;
			actorJson["parent"] = a->parent->uid;

			//Setup each component
			for (IComponent* c : a->components)
			{
				json comp;

				comp["type"] = c->TypeName();
				c->SetJson(comp["properties"]);

				actorJson["components"].push_back(comp);
			}

			//Add to actor array
			j["actors"].push_back(actorJson);
		}

		//Save json to file
		std::ofstream os(path, std::ofstream::trunc);
		os << j.dump(4);
		os.close();
	}
}
