#pragma once

#include <Core/Core.h>
#include <Core/Application/Application.h>
#include <Core/EventSystem/Events.h>
#include <Core/Debug/Log.h>
#include "Core/Graphics/Renderer.h"
#include "ServiceLocator.h"

namespace LilEngie
{
	typedef void(*Function)();

	class LIL_API Game : IEventListener
	{
	private:
		//Sub systems and services
		ServiceLocator serviceLocator;
		Application application;
		EventManager eventManager;
		Renderer renderer;
		Log log;

		bool isRunning = true;
		Event closeEvent;

	public:
		Game(Function start, Function update);
		~Game();

		void OnEvent(const Event &e) override;
	};
}
