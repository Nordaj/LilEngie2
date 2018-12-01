#include <Core/Game/ServiceLocator.h>
#include "EventManager.h"
#include "Event.h"
#include "IEventListener.h"

namespace LilEngie
{
	IEventListener::~IEventListener()
	{
		SERVICES_GET(EventManager)->RemoveListener(this);
	}

	void IEventListener::Subscribe(EventType type)
	{
		SERVICES_GET(EventManager)->Subscribe(type, this);
	}

	void IEventListener::UnSubscribe(EventType type)
	{
		SERVICES_GET(EventManager)->UnSubscribe(type, this);
	}
}
