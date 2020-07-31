#include <string>
#include <LilEngie.h>
#include "LilEddieGame.h"
#include <windows.h>

namespace LilEddie
{
	LilEddieGame::LilEddieGame()
		: windowManager(this)
	{
		//Temporary, later will be main function argument for opening specific projects, could be anywhere
		gamePath = "../Sandbox/";
		projName = "Sandbox";

		//Create default component factory
		componentFactory = new ComponentFactory();
		componentFactory->InitComponentList();
		ComponentFactory::core = componentFactory;

		//Allow us to load game dll with its component factory if a build exists
		ReloadGameDLL();
	}

	LilEddieGame::~LilEddieGame()
	{
		
	}

	void LilEddieGame::ReloadGameDLL()
	{
		//Use config as string for dll path
		std::string config = "Release";
	#ifdef LIL_DEBUG
		config = "Debug";
	#endif

		//Get dll path as wstring
		std::string str = gamePath + "bin/" + config + "/x64/" + projName + ".dll";
		std::wstring wstr = std::wstring(str.begin(), str.end());

		//Load the game dll
		HINSTANCE tempGameLib = LoadLibrary(wstr.c_str());
		if (!tempGameLib)
		{
			LIL_ERROR("Could not find or link game dll.");
			return;
		}

		//Get address of CreateComponentFactory function
		void* createCompAddr = GetProcAddress(tempGameLib, "CreateComponentFactory");
		if (!createCompAddr)
		{
			LIL_ERROR("Could not find CreateComponentFactory function of game, will not link new dll.");
			FreeLibrary(tempGameLib);
			return;
		}

		//Delete old component factory if it's there
		if (ComponentFactory::core)
			delete ComponentFactory::core;

		//Create and setup the component factory
		auto gameCreateCompFactory = (ComponentFactory* (*)())createCompAddr;
		componentFactory = gameCreateCompFactory();
		componentFactory->InitComponentList();
		ComponentFactory::core = componentFactory;

		//Update game lib in current use
		if (gameLib)
			FreeLibrary((HMODULE)gameLib);
		gameLib = tempGameLib;
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
