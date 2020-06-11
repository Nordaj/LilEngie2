#include "IComponent.h"
#include "Scene.h"
#include "SceneManager.h"
#include "ComponentList.h"
#include "Actor.h"

namespace LilEngie
{
	Actor::Actor(Scene* scene)
		: scene(scene)
	{ 
		children = std::vector<Actor*>();
		components = std::vector<IComponent*>();
	}

	Actor::~Actor()
	{
		for (int i = 0; i < components.size(); i++)
			delete components[i];
		components.clear();
	}

	int Actor::GetChildrenCount()
	{
		return children.size();
	}
	
	Actor* Actor::GetChild(int index)
	{
		return children[index];
	}

	void Actor::SetEnabled(bool val)
	{
		isEnabled = true;
	}

	bool Actor::IsEnabled()
	{
		return isEnabled;
	}

	void Actor::Destroy()
	{
		scene->DestroyActor(this);
	}

	void Actor::DestroyComponent(IComponent* component)
	{
		for (int i = 0; i < components.size(); i++)
		{
			if (components[i] == component)
			{
				delete component;
				components.erase(components.begin() + i);
			}
		}
	}

	void Actor::Deserialize(json& j)
	{
		DeserializeProperty(j, name, "name");
		parent = scene->GetActor(j["parent"]);

		//Handle each component
		for (auto& c : j["components"])
		{
			IComponent* comp = CreateComponentFromString(this, c["type"]);
			comp->Deserialize(c["properties"]);
			if (c["type"] == "transform") transform = (TransformComponent*)comp;
			comp->Init();
		}
	}

	void Actor::Serialize(json& j)
	{
		SerializeProperty(j, name, "name");
		SerializeProperty(j, uid, "uid");
		SerializeProperty(j, parent->uid, "parent");

		//Serialize each component
		for (IComponent* comp : components)
		{
			json component;
			component["type"] = comp->TypeName();
			comp->Serialize(component["properties"]);
			j["components"].push_back(component);
		}
	}

	void Actor::Update()
	{
		for (int i = 0; i < components.size(); i++)
			components[i]->Update();

		for (int i = 0; i < children.size(); i++)
			children[i]->Update();
	}

	void Actor::OnDraw()
	{
		for (int i = 0; i < components.size(); i++)
			components[i]->OnDraw();

		for (int i = 0; i < children.size(); i++)
			children[i]->OnDraw();
	}

	void Actor::Start()
	{
		for (int i = 0; i < components.size(); i++)
			components[i]->Start();

		for (int i = 0; i < children.size(); i++)
			children[i]->Start();
	}

	Actor* ActorRef::Get()
	{
		if (actor) 
			return actor;

		actor = SceneManager::core->scene->GetActor(uid);
		return actor;
	}

	Actor* ActorRef::operator->()
	{
		return Get();
	}
}
