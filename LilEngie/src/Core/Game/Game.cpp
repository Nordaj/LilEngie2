#include <iostream>
#include <functional>
#include <memory>
#include <Core/Application/Application.h>
#include <Core/EventSystem/Events.h>
#include <Core/Debug/Log.h>
#include <Core/Debug/DebugTimer.h>
#include <Core/System/PoolAllocator.h>
#include <Core/Platform/Graphics/IGraphics.h>
#include <Core/Entity/SceneManager.h>
#include <Core/Entity/Scene.h>
#include <Core/Entity/Actor.h>
#include <Core/Entity/CoreComponents/MeshComponent.h>
#include <Core/Entity/CoreComponents/TransformComponent.h>
#include <Core/Entity/CoreComponents/CameraComponent.h>
#include <Core/Resources/Types/MeshResource.h>
#include <Core/Resources/Types/ShaderResource.h>
#include <Core/Resources/Types/MaterialResource.h>
#include <Core/Graphics/Material.h>
#include <Core/Resources/Types/TextureResource.h>
#include <Core/Math/LilMath.h>
#include "Game.h"

namespace LilEngie
{
	Game::Game()
	{ }

	Game::~Game()
	{ 
		delete componentFactory;
	}

	void Game::Run()
	{
		//Set core pointers
		ResourceManager::core = &resourceManager;
		EventManager::core = &eventManager;

		//Setup events
		closeEvent = Event(EventType::GameClose);

		//Logging
		Log::core = &logger;
		logger.verbosity = Verbosity::Verbose; //Remove eventually

		//Setup component factory if not setup by derrived
		if (!componentFactory)
		{
			componentFactory = new ComponentFactory();
			componentFactory->InitComponentList();
			componentFactory->core = componentFactory;
		}

		//Initialization
		sceneManager.Init(this);
		resourceManager.Init(this);
		application.Init();
		renderer.Init(application.windowProperties, GraphicsAPI::DirectX11, this);
		input.Init(this);

		Init();

		//Subscribe to any necessary events
		Subscribe(EventType::WindowClose);

		//Load scene from file
		sceneManager.LoadScene("res/Scenes/Test.lilscn");

		//sceneManager.SaveScene("res/Scenes/TestSave.lilscn");

		//Main loop
		Start();
		DebugTimer t;
		while (isRunning)
		{
			input.Update();
			application.Update();

			Update();
			sceneManager.scene->DispatchActorEvent(ActorEvent::Update);

			sceneManager.scene->DispatchActorEvent(ActorEvent::OnDraw);
			sceneManager.scene->DispatchActorEvent(ActorEvent::OnDrawImGui);

			renderer.Render();

			deltaTime = t.GetElapsed(TimeUnit::Seconds);
			t = DebugTimer();
		}

		//Shutdown
		sceneManager.Shutdown();
		resourceManager.UnloadAllResouces();
		eventManager.Dispatch(closeEvent);
		renderer.Shutdown();
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
