#pragma once

#include <Core/Core.h>

namespace LilEngie
{
	class EventManager;
	class Game;

	class LIL_API ServiceLocator
	{
		//Game is responsible for maintaining each pointer
		friend class Game;

	private:
		static EventManager * eventManager;

	public:
		inline static EventManager* EventManager()
		{ return eventManager; }
	};

	typedef ServiceLocator Services;
}

#define SERVICES_GET(x) if (Services::x() != nullptr) Services::x()
