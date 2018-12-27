#pragma once

#include <Core/Core.h>

namespace LilEngie
{
	class Event;

	class LIL_API IEventListener
	{
	public:
		IEventListener() {}
		virtual ~IEventListener();

		virtual void OnEvent(const Event &e) = 0;

	protected:
		void Subscribe(EventType type);
		void UnSubscribe(EventType type);
	};
}
