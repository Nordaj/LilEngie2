#include "IComponent.h"
#include "Scene.h"
#include "SceneManager.h"
#include "ComponentFactory.h"
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

	void Actor::DispatchActorEvent(ActorEvent type)
	{
		switch (type)
		{
			case ActorEvent::Init:
				for (IComponent* comp : components)
					comp->Init();
				break;
			case ActorEvent::Start:
				for (IComponent* comp : components)
					comp->Start();
				break;
			case ActorEvent::Update:
				for (IComponent* comp : components)
					comp->Update();
				break;
			case ActorEvent::OnDraw:
				for (IComponent* comp : components)
					comp->OnDraw();
				break;
			case ActorEvent::EditorUpdate:
				for (IComponent* comp : components)
					comp->EditorUpdate();
				break;
			case ActorEvent::OnDrawImGui:
				for (IComponent* comp : components)
					comp->OnDrawImGui();
				break;
			default:
				break;
		}

		for (int i = 0; i < children.size(); i++)
			children[i]->DispatchActorEvent(type);
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

	bool Actor::ContainsComponents(std::vector<std::string>& ids)
	{
		for (std::string& str : ids)
		{
			if (!ContainsComponent(str))
				return false;
		}
		return true;
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
		DeserializeProperty(j, editorOnly, "editorOnly");
		DeserializeProperty(j, hideInTree, "hideInTree");
		scene->SetActorParent(this, scene->GetActor(j["parent"]));

		//Handle each component
		for (auto& c : j["components"])
		{
			//Try to create component from engine or game, otherwise skip over
			std::string type = c["type"];
			IComponent* comp = ComponentFactory::core->CreateComponent(this, type.c_str());
			if (!comp)
			{
				LIL_WARN("Could not instantiate component type: ", c["type"]);
				continue;
			}

			comp->Deserialize(c["properties"]);
		}
	}

	void Actor::Serialize(json& j)
	{
		SerializeProperty(j, name, "name");
		SerializeProperty(j, uid, "uid");
		if (parent)
			SerializeProperty(j, parent->uid, "parent");
		SerializeProperty(j, editorOnly, "editorOnly");
		SerializeProperty(j, hideInTree, "hideInTree");

		//Serialize each component
		for (IComponent* comp : components)
		{
			json component;
			component["type"] = comp->TypeName();
			comp->Serialize(component["properties"]);
			j["components"].push_back(component);
		}
	}
}
