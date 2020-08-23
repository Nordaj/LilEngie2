#pragma once

#include <string>
#include <Core/Core.h>
#include <Core/System/ISerializable.h>
#include <Core/Application/Application.h>
#include <Core/EventSystem/Events.h>
#include <Core/Debug/Log.h>
#include <Core/Resources/ResourceManager.h>
#include <Core/Graphics/Renderer.h>
#include <Core/Entity/SceneManager.h>
#include "Input.h"

namespace LilEngie
{
	class ComponentFactory;

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
		ComponentFactory* componentFactory;
		float deltaTime;

		std::string gamePath;

	protected:
		bool playMode;

	private:
		json cachedScene;
		bool isRunning = true;
		Event closeEvent;

	public:
		Game();
		virtual ~Game();

		void Run();
		void OnEvent(const Event &e) override;

		bool IsPlaying();
		void EnterPlayMode();
		void ExitPlayMode();
		void SetPlayMode(bool isPlay);

		void SetCorePtrs();

	protected:
		virtual void Init() {}
		virtual void Start() {}
		virtual void Update() {}
	};
}
