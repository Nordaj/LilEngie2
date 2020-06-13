#include <Core/EventSystem/Events.h>
#include <Core/Game/Game.h>
#include "Actor.h"
#include "IComponent.h"

namespace LilEngie
{
	void IComponent::Destroy()
	{
		actor->DestroyComponent(this);
	}

	bool IComponent::GetKey(Key key)
	{
		return Input::core->GetKey(key);
	}

	bool IComponent::GetKeyDown(Key key)
	{
		return Input::core->GetKeyDown(key);
	}

	bool IComponent::GetKeyUp(Key key)
	{
		return Input::core->GetKeyUp(key);
	}
}
