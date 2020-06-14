#include <Core/EventSystem/Events.h>
#include <Core/Game/Game.h>
#include "Actor.h"
#include <Core/Platform/Window/MouseState.h>
#include <Core/Platform/Window/Keys.h>
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
}
