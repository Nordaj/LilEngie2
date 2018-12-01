#include <iostream>
#include <functional>
#include <memory>
#include <Core/Application/Application.h>
#include <Core/EventSystem/Events.h>
#include "Game.h"

namespace LilEngie
{
	Game::Game(Function start, Function update)
	{
		//Setup events
		closeEvent = Event(EventType::GameClose);

		//Connect global services to service locator
		ServiceLocator::eventManager = &eventManager;

		//Initialization
		application.Init();

		//Subscribe to any necessary events
		Subscribe(EventType::WindowClose);

		if (start) start();
		while (isRunning)
		{
			if (update) update();
			application.Update();
		}
		SERVICES_GET(EventManager)->Dispatch(closeEvent);
	}

	Game::~Game()
	{
		//Empty services from service locator
		ServiceLocator::eventManager = nullptr;
	}

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
