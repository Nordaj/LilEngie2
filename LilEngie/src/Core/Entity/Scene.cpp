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
}
