#include "IComponent.h"
#include "Scene.h"
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

	void Actor::Update()
	{
		for (int i = 0; i < components.size(); i++)
			components[i]->Update();

		for (int i = 0; i < children.size(); i++)
			children[i]->Update();
	}
}
