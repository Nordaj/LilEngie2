#pragma once

#include <Core/Core.h>

namespace LilEngie
{
	class EventManager;
	class Log;
	class Game;

	class LIL_API ServiceLocator
	{
		//Game is responsible for maintaining each pointer
		friend class Game;

	private:
		static EventManager *eventManager;
		static Log *log;

	public:
		inline static EventManager *EventManager()
		{ return eventManager; }

		inline static Log *Log()
		{ return log; }
	};

	typedef ServiceLocator Services;
}

//Both do the same thing, pick a favorite
#define SERVICES_GET(x) if (LilEngie::Services::x() != nullptr) LilEngie::Services::x()
#define LIL(x) if (LilEngie::Services::x() != nullptr) LilEngie::Services::x()
