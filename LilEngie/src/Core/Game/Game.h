#pragma once

#include <Core/Core.h>
#include <Core/Application/Application.h>
#include <Core/EventSystem/Events.h>
#include <Core/Debug/Log.h>
#include <Core/Resources/ResourceManager.h>
#include "Core/Graphics/Renderer.h"
#include "Core/Entity/SceneManager.h"
#include "Input.h"

namespace LilEngie
{
	typedef void(*Function)();

	class LIL_API Game : IEventListener
	{
	public:
		EventManager eventManager;
		Application application;
		Renderer renderer;
		Log logger;
		ResourceManager resourceManager;
		SceneManager sceneManager;
		Input input;
		float deltaTime;

	private:
		Function start, update, init;

		bool isRunning = true;
		Event closeEvent;

	public:
		Game(Function start, Function update, Function init);
		~Game();

		void Run();
		void OnEvent(const Event &e) override;
	};
}
