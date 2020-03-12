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
		SceneManager::core = &sceneManager;

		//Setup events
		closeEvent = Event(EventType::GameClose);

		//Logging
		Log::core = &logger;
		logger.verbosity = Verbosity::Verbose; //Remove eventually

		//Initialization
		application.Init();
		renderer.Init(application.windowProperties, GraphicsAPI::OpenGL);

		//Subscribe to any necessary events
		Subscribe(EventType::WindowClose);

		//TEST BENCH


		
		//Setup main scene
		Scene* mainScene = new Scene();
		sceneManager.scene = mainScene;
		mainScene->Init();

		//Create mesh actor
		{
			Actor* actor = mainScene->CreateActor();

			TransformComponent* tc = actor->CreateComponent<TransformComponent>();
			MeshComponent* mc = actor->CreateComponent<MeshComponent>();

			std::string path("LilEngie/res/Models/square.obj");
			ResourceId id = ResourceId(path, ResourceType::Mesh);
			MeshResource* meshResource = (MeshResource*)ResourceManager::core->LoadResource(id);

			mc->meshRenderer.meshResId = id;
		}

		//Create camera actor
		{
			Actor* cameraActor = mainScene->CreateActor();

			TransformComponent* tc = cameraActor->CreateComponent<TransformComponent>();
			CameraComponent* cc = cameraActor->CreateComponent<CameraComponent>();

			//TODO, wont need to mult by -1 once inverse support
			translate(tc->mat, vec3(0, 0, -6) * -1);
		}

		mainScene->Start();



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
