#include <cstdio>
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
		std::string config = "Release";
	#ifdef LIL_DEBUG
		config = "Debug";
	#endif

		//Create path to game module
		std::string gameOutput = gamePath + "bin/" + config + "/x64/";
		std::string modulePath = gameOutput + "LinkedModules/";
		
		//TODO: need to make sure that when fail, new path is going to be deleted

		//Create module path directory in case it doesnt exist
		std::error_code pathErr;
		std::filesystem::create_directory(modulePath, pathErr);

		//Setup dll and pdb paths
		std::string modNum = std::to_string(currentModuleNumber++);
		std::string dllPath = modulePath + projName + modNum + ".dll";
		std::string pdbPath = modulePath + projName + modNum + ".pdb";

		//Copy the dll to the new path
		std::error_code dllErr, pdbErr; ///I dont want exceptions if the copies fail
		std::filesystem::copy(gameOutput + projName + ".dll", dllPath,
			std::filesystem::copy_options::overwrite_existing, dllErr);
		std::filesystem::copy(gameOutput + projName + ".pdb", pdbPath,
			std::filesystem::copy_options::overwrite_existing, pdbErr);

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

		//Update global core pointers on both dll's to match
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
