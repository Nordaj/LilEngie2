#include <Core/EventSystem/Events.h>
#include "Actor.h"
#include "Component.h"

namespace LilEngie
{
	void Component::Destroy()
	{
		actor->DestroyComponent(this);
	}
}
