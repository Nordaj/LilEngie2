#include <string>
#include <map>
#include "Actor.h"
#include "Scene.h"

namespace LilEngie
{
	Scene::Scene()
	{ }

	Scene::~Scene()
	{
		if (!isCleaned && isInitialized)
			Clean();
	}

	void Scene::Init(bool enable)
	{
		//Create the root object
		root = new Actor(this);
		root->name = "ROOT";
		root->uid = "ROOT";

		actors["ROOT"] = root;

		isEnabled = enable;
		isInitialized = true;
	}

	bool Scene::IsIntialized()
	{ return isInitialized; }

	void Scene::Clean()
	{
		//Might or might not be what i want
		if (!isCleaned && isInitialized)
			delete root;

		isEnabled = false;
		isInitialized = false;
		isCleaned = true;
	}

	void Scene::SetEnabled(bool val)
	{
		if (isInitialized)
			isEnabled = val;
	}

	bool Scene::IsEnabled()
	{ return isEnabled; }

	void Scene::Start()
	{
		if (isEnabled)
			root->Start();
	}

	void Scene::Update()
	{
		//Sort of a slow method to updating the scene.
		//May want to look into faster alternatives
		if (isEnabled)
			root->Update();
	}

	void Scene::OnDraw()
	{
		if (isEnabled)
			root->OnDraw();
	}

	void Scene::OnDrawImGui()
	{
	#ifdef LIL_ENABLE_IMGUI
		if (isEnabled)
			root->OnDrawImGui();
	#endif //LIL_ENABLE_IMGUI
	}

	Actor* Scene::CreateActor(std::string uid, Actor* parent)
	{
		if (isInitialized)
		{
			if (parent == nullptr)
				parent = root;

			//Actor creation
			Actor *a = new Actor(this);
			parent->children.push_back(a);
			a->parent = parent;
			a->uid = uid;

			//Keep track of it
			actors[uid] = a;

			return a;
		}
		return nullptr;
	}

	void Scene::DestroyActor(Actor* actor)
	{
		if (isInitialized)
		{
			//Remove actor from parents child list
			for (int i = 0; i < actor->parent->children.size(); i++)
			{
				if (actor->parent->children[i] == actor)
					actor->parent->children.erase(actor->parent->children.begin() + i);
			}

			//Recursively destroy each child starting from end
			if (actor->children.size() > 0)
				DestroyActor(actor->children[0]);

			//Free actor memory, this will also free all components
			delete actor;
		}
	}

	Actor* Scene::GetActor(std::string uid)
	{
		if (actors.find(uid) != actors.end())
			return actors[uid];
		return nullptr;
	}

	void Scene::Deserialize(json& j)
	{
		//Start by creating actors manually with the uid's
		for (auto& a : j["actors"])
			Actor* actor = CreateActor(a["uid"]);

		//Deserialize each actor
		for (auto& a : j["actors"])
		{
			Actor* actor = GetActor(a["uid"]);
			actor->Deserialize(a);
		}
	}

	void Scene::Serialize(json& j)
	{
		for (auto it = actors.begin(); it != actors.end(); it++)
		{
			//Skip root
			if (it->second->uid == "ROOT")
				continue;

			json actor;
			it->second->Serialize(actor);
			j["actors"].push_back(actor);
		}
	}
}
