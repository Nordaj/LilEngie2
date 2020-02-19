#include <iostream>
#include <functional>
#include <memory>
#include <Core/Application/Application.h>
#include <Core/EventSystem/Events.h>
#include <Core/Debug/Log.h>
#include <Core/Debug/DebugTimer.h>
#include <Core/System/PoolAllocator.h>
#include <Core/Platform/Graphics/IGraphics.h>

#include <Core/Math/vec3.h>

#include "Game.h"


namespace LilEngie
{
	Game::Game(Function start, Function update)
	{
		//Set core pointers
		Renderer::core = &renderer;
		ResourceManager::core = &resourceManager;
		EventManager::core = &eventManager;

		//Setup events
		closeEvent = Event(EventType::GameClose);

		//Logging
		Log::core = &logger;
		logger.verbosity = Verbosity::Verbose; //Remove eventually

		//Initialization
		application.Init();
		renderer.Init(application.windowProperties, GraphicsAPI::DirectX11);

		//Subscribe to any necessary events
		Subscribe(EventType::WindowClose);

		//TEST BENCH
		

		//Main loop
		if (start) start();
		while (isRunning)
		{
			if (update) update();
			application.Update();

			renderer.Render();
		}

		//Shutdown
		resourceManager.UnloadAllResouces();
		eventManager.Dispatch(closeEvent);
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
