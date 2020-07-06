#include <string>
#include <map>
#include <Core/Core.h>
#include <Core/Debug/Log.h>
#include "IComponent.h"
#include "Actor.h"
#include "CoreComponents/TransformComponent.h"
#include "CoreComponents/CameraComponent.h"
#include "CoreComponents/MeshComponent.h"
#include "ComponentFactory.h"

namespace LilEngie
{
	ComponentFactory* ComponentFactory::core = nullptr;

	void ComponentFactory::InitComponentList()
	{
		componentNames = std::vector<std::string>{
			"transform",
			"camera",
			"mesh"
		};

		InitGameComponentList();
	}

	IComponent* ComponentFactory::CreateComponent(Actor* actor, const char* type)
	{
		if (!strcmp(type, "transform"))
			return actor->CreateComponent<TransformComponent>();
		else if (!strcmp(type, "camera"))
			return actor->CreateComponent<CameraComponent>();
		else if (!strcmp(type, "mesh"))
			return actor->CreateComponent<MeshComponent>();

		return CreateGameComponent(actor, type);
	}

	void ComponentFactory::InitGameComponentList()
	{

	}

	IComponent* ComponentFactory::CreateGameComponent(Actor* actor, const char* type)
	{
		return nullptr;
	}
}
