#pragma once

#include <Core/Core.h>
#include "Event.h"

namespace LilEngie
{
	class Event;

	class LIL_API IEventListener
	{
	public:
		IEventListener() {}
		virtual ~IEventListener();

		virtual void OnEvent(const Event& e);

	protected:
		void Subscribe(EventType type);
		void UnSubscribe(EventType type);
	};
}
