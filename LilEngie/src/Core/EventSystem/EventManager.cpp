#include "Event.h"
#include "EventManager.h"

namespace LilEngie
{
	void EventManager::Dispatch(const Event &e) const
	{
		for (int i = 0; i < listeners[e.type].size(); i++)
			listeners[e.type][i]->OnEvent(e);
	}

	void EventManager::Subscribe(EventType type, IEventListener *listener)
	{
		listeners[type].push_back(listener);
	}

	void EventManager::UnSubscribe(EventType type, IEventListener *listener)
	{
		for (int i = 0; i < listeners[type].size(); i++)
		{
			if (listeners[type][i] == listener)
				listeners[type].erase(listeners[type].begin() + i);
		}
	}

	void EventManager::RemoveListener(IEventListener *listener)
	{
		//vector removal is slow, might want to consider setting to nullptr
		for (int i = 0; i < EventType::EventTypeCount; i++)
		{
			for (int j = 0; j < listeners[i].size(); j++)
			{
				if (listeners[i][j] == listener)
					listeners[i].erase(listeners[i].begin() + j);
			}
		}
	}
}
