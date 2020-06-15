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
	Game* Game::core = nullptr;

	Game::Game(Function start, Function update, Function init)
		: start(start), update(update), init(init), deltaTime(0)
	{ 
		core = this;
	}

	Game::~Game()
	{ }

	void Game::Run()
	{
		//Set core pointers
		Renderer::core = &renderer;
		ResourceManager::core = &resourceManager;
		EventManager::core = &eventManager;
		SceneManager::core = &sceneManager;

		//Setup events
		closeEvent = Event(EventType::GameClose);

		//Logging
		Log::core = &logger;
		logger.verbosity = Verbosity::Verbose; //Remove eventually

		//Initialization
		application.Init();
		renderer.Init(application.windowProperties, GraphicsAPI::OpenGL);
		input.Init(this);

		if (init) init();

		//Subscribe to any necessary events
		Subscribe(EventType::WindowClose);

		//Load scene from file
		sceneManager.LoadScene("LilEngie/res/Scenes/Test.lilscn");

		//sceneManager.SaveScene("LilEngie/res/Scenes/TestSave.lilscn");

		//Main loop
		if (start) start();
		DebugTimer t;
		while (isRunning)
		{
			input.Update();
			application.Update();

			if (update) update();
			sceneManager.scene->Update();

			sceneManager.scene->OnDraw();
			sceneManager.scene->OnDrawImGui();
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
