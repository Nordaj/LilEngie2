#include <iostream>
#include <functional>
#include <memory>
#include <Core/Application/Application.h>
#include <Core/EventSystem/Events.h>
#include <Core/Debug/Log.h>
#include <Core/Debug/DebugTimer.h>
#include <Core/System/PoolAllocator.h>
#include <Core/System/ISerializable.h>
#include <Core/Platform/Graphics/IGraphics.h>
#include <Core/Entity/SceneManager.h>
#include <Core/Entity/Scene.h>
#include <Core/Entity/Actor.h>
#include <Core/Entity/ComponentFactory.h>
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
		: playMode(true)
	{ }

	Game::~Game()
	{ 
		delete componentFactory;
	}

	void Game::Run()
	{
		//Set all core engine subsystem pointers
		SetCorePtrs();

		//Setup events
		closeEvent = Event(EventType::GameClose);

		//Logging
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

		//Main loop
		Start();
		if (playMode)
			sceneManager.scene->DispatchActorEvent(ActorEvent::Start);
		DebugTimer t;
		while (isRunning)
		{
			input.Update();
			application.Update();

			Update();

			if (playMode)
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

	bool Game::IsPlaying()
	{
		return playMode;
	}

	void Game::EnterPlayMode()
	{
		//Cache scene
		cachedScene.clear();
		sceneManager.scene->Serialize(cachedScene);

		//Dispatch play mode event
		Event e = Event();
		e.type = EventType::EnterPlayMode;
		eventManager.Dispatch(e);

		//Enter play mode
		playMode = true;
		sceneManager.scene->DispatchActorEvent(ActorEvent::Start);
	}

	void Game::ExitPlayMode()
	{
		playMode = false;

		//Reload cached scene
		sceneManager.LoadScene(cachedScene);

		//Dispatch play mode event
		Event e = Event();
		e.type = EventType::ExitPlayMode;
		eventManager.Dispatch(e);
	}

	void Game::SetPlayMode(bool isPlay)
	{
		if (playMode && !isPlay)
			ExitPlayMode();
		else if (!playMode && isPlay)
			EnterPlayMode();
	}

	void Game::SetCorePtrs()
	{
		ResourceManager::core = &resourceManager;
		EventManager::core = &eventManager;
		Log::core = &logger;
		Input::core = &input;

		if (componentFactory)
			ComponentFactory::core = componentFactory;
	}
}
