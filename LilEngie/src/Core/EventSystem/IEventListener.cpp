#include "EventManager.h"
#include "Event.h"
#include "IEventListener.h"

namespace LilEngie
{
	IEventListener::~IEventListener()
	{
		EventManager::core->RemoveListener(this);
	}

	void IEventListener::OnEvent(const Event& e)
	{
		//Do nothing
	}

	void IEventListener::Subscribe(EventType type)
	{
		EventManager::core->Subscribe(type, this);
	}

	void IEventListener::UnSubscribe(EventType type)
	{
		EventManager::core->UnSubscribe(type, this);
	}
}
