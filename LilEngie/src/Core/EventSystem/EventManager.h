#pragma once

#include <Core/Core.h>
#include <vector>
#include "Event.h"
#include "IEventListener.h"

namespace LilEngie
{
	class LIL_API EventManager
	{
	private:
		std::vector<IEventListener*> listeners[EventType::EventTypeCount];

	public:
		void Dispatch(const Event &e) const;
		void Subscribe(EventType type, IEventListener *listener);
		void UnSubscribe(EventType type, IEventListener *listener);
		void RemoveListener(IEventListener *listener);
	};
}
