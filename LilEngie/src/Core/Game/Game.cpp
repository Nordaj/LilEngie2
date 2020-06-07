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

#include <Core/System/ISerializable.h>

#include "Game.h"

namespace LilEngie
{
	Game::Game(Function start, Function update)
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
		renderer.Init(application.windowProperties, GraphicsAPI::DirectX11);

		//Subscribe to any necessary events
		Subscribe(EventType::WindowClose);

		//Load scene from file
		sceneManager.LoadScene("LilEngie/res/Scenes/Test.lilscn");

		//Main loop
		if (start) start();
		while (isRunning)
		{
			if (update) update();
			application.Update();

			sceneManager.scene->Update();

			sceneManager.scene->OnDraw();
			renderer.Render();
		}

		//Shutdown
		sceneManager.Shutdown();
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
