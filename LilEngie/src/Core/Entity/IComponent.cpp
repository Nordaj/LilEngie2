#include <Core/EventSystem/Events.h>
#include "Actor.h"
#include "IComponent.h"

namespace LilEngie
{
	void IComponent::Destroy()
	{
		actor->DestroyComponent(this);
	}
}
