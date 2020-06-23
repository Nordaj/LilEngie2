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
		game = scene->manager->game;
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

	bool Actor::ContainsComponent(std::string id)
	{
		for (IComponent* comp : components)
		{
			if (comp->TypeName() == id)
				return true;
		}
		return false;
	}

	int Actor::ComponentsCount()
	{
		return components.size();
	}

	IComponent* Actor::GetComponent(int i)
	{
		return components[i];
	}

	void Actor::Deserialize(json& j)
	{
		DeserializeProperty(j, name, "name");
		scene->SetActorParent(this, scene->GetActor(j["parent"]));

		//Handle each component
		for (auto& c : j["components"])
		{
			//Try to create component from engine or game, otherwise skip over
			IComponent* comp = CreateComponentFromString(this, c["type"]);
			if (!comp && scene->manager->gameComponentFactory)
				comp = scene->manager->gameComponentFactory(this, c["type"]);
			if (!comp)
			{
				LIL_WARN("Could not instantiate component type: ", c["type"]);
				continue;
			}

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

	void Actor::OnDrawImGui()
	{
		for (int i = 0; i < components.size(); i++)
			components[i]->OnDrawImGui();

		for (int i = 0; i < children.size(); i++)
			children[i]->OnDrawImGui();
	}

	void Actor::Start()
	{
		for (int i = 0; i < components.size(); i++)
			components[i]->Start();

		for (int i = 0; i < children.size(); i++)
			children[i]->Start();
	}
}
