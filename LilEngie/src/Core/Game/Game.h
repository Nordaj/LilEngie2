#pragma once

#include <Core/Core.h>
#include <Core/Application/Application.h>
#include <Core/EventSystem/Events.h>
#include <Core/Debug/Log.h>
#include "Core/Graphics/Renderer.h"

namespace LilEngie
{
	typedef void(*Function)();

	class LIL_API Game : IEventListener
	{
	public:
		EventManager eventManager;

	private:
		Application application;
		Renderer renderer;
		Log logger;

		bool isRunning = true;
		Event closeEvent;

	public:
		Game(Function start, Function update);
		~Game();

		void OnEvent(const Event &e) override;
	};
}
