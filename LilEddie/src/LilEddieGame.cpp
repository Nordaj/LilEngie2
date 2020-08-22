#include <stdio.h>
#include <string>
#include <filesystem>
#include <LilEngie.h>
#include "LilEddieGame.h"

#include <windows.h>
#include <fileapi.h>

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
		std::string config = "EditorGameRelease";
	#ifdef LIL_DEBUG
		config = "EditorGameDebug";
	#endif

		//Create path to game module
		std::string gameOutput = gamePath + "bin/" + config + "/x64/";

		//Find Game DLL Path using LatestVersion.txt
		FILE* versFile = fopen((gameOutput + "LatestVersion.txt").c_str(), "r");
		if (!versFile)
		{
			LIL_ERROR("Could not open LatestVersion.txt file to find game module.");
			return;
		}

		std::string versLn(128, 0);
		if (!fgets(&versLn[0], 128, versFile))
		{
			LIL_ERROR("LatestVersion.txt was empty.");
			return;
		}
		if (auto p = versLn.find("\n"))
			versLn.erase(p);

		std::string dllPath = gameOutput + versLn + ".dll";
		fclose(versFile);

		//Load the game dll
		std::wstring wDllPath = std::wstring(dllPath.begin(), dllPath.end());
		HINSTANCE tempGameLib = LoadLibrary(wDllPath.c_str());
		if (!tempGameLib)
		{
			LIL_ERROR("Could not find or link game dll.");
			return;
		}

		//Get address of CreateComponentFactory function
		void* createCompAddr = GetProcAddress(tempGameLib, "CreateComponentFactory");
		if (!createCompAddr)
		{
			LIL_ERROR("Could not find CreateComponentFactory function of game module, will not link new dll.");
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

		//Update global core pointers on both ends to match
		SetCorePtrs();
		void* syncCorePtrs = GetProcAddress(tempGameLib, "SyncGlobalPtrs");
		if (!syncCorePtrs)
		{
			LIL_ERROR("Could not find core pointer syncing function of game module, will not link new dll.");
			FreeLibrary(tempGameLib);
			return;
		}
		((void(*)(Game*))syncCorePtrs)(this);

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
