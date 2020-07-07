#include <LilEngie.h>
#include "LilEddieGame.h"

namespace LilEddie
{
	LilEddieGame::LilEddieGame()
		: windowManager(this)
	{
		//Make game use our own component factory
		componentFactory = new ComponentFactory();
		componentFactory->InitComponentList();
		ComponentFactory::core = componentFactory;

		gamePath = "../Sandbox/";
	}

	LilEddieGame::~LilEddieGame()
	{
		
	}

	void LilEddieGame::Init()
	{
		//Sloppy solution to start maximized, only temp
		application.window.Maximize();
	}

	void LilEddieGame::Start()
	{
		LIL_LOG("Hello, welcome to Lil Engie's editor - Lil Eddie");

		windowManager.Init();
	}

	void LilEddieGame::Update()
	{
		sceneManager.scene->DispatchActorEvent(ActorEvent::EditorUpdate);
		windowManager.Update();
	}
}
