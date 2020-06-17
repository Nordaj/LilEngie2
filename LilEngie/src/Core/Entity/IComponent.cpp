#include <string>
#include <Core/EventSystem/Events.h>
#include <Core/Game/Game.h>
#include <Core/Platform/Window/MouseState.h>
#include <Core/Platform/Window/Keys.h>
#include "Actor.h"
#include "Scene.h"
#include "IComponent.h"

namespace LilEngie
{
	IComponent::IComponent()
	{
		actor = nullptr;
		input = Input::core;
	}

	void IComponent::Destroy()
	{
		actor->DestroyComponent(this);
	}

	Actor* IComponent::GetActor(std::string& uid)
	{
		return actor->scene->GetActor(uid);
	}

	float IComponent::DeltaTime()
	{
		return actor->game->deltaTime;
	}
}
