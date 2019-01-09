#include <iostream>
#include <functional>
#include <memory>
#include <Core/Application/Application.h>
#include <Core/EventSystem/Events.h>
#include <Core/Debug/Log.h>
#include <Core/Debug/DebugTimer.h>
#include <Core/System/PoolAllocator.h>
#include <Core/Platform/Graphics/IGraphics.h>
#include "Game.h"

namespace LilEngie
{
	Game::Game(Function start, Function update)
	{
		//Setup events
		closeEvent = Event(EventType::GameClose);

		//Connect global services to service locator
		serviceLocator.eventManager = &eventManager;
		serviceLocator.log = &log;

		//Initialization
		application.Init();
		renderer.Init(application.windowProperties, GraphicsAPI::OpenGL);

		//Subscribe to any necessary events
		Subscribe(EventType::WindowClose);

		//Main loop
		if (start) start();
		while (isRunning)
		{
			if (update) update();
			application.Update();

			renderer.Render();
		}

		//Shutdown
		SERVICES_GET(EventManager)->Dispatch(closeEvent);
		renderer.Shutdown();
	}

	Game::~Game()
	{ }

	void Game::OnEvent(const Event &e)
	{
		switch (e.type)
		{
			case EventType::WindowClose:
				isRunning = false;
				break;
			default:
				break;
		}
	}
}
