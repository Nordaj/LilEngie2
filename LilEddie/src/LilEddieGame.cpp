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
		slnPath = "../LilEngie2.sln";

		//Create default component factory
		componentFactory = new ComponentFactory();
		componentFactory->InitComponentList();
		ComponentFactory::core = componentFactory;

		//Should not be in play mode by default
		playMode = false;

		//Allow us to load game dll with its component factory if a build exists
		ReloadGameDLL();
	}

	LilEddieGame::~LilEddieGame()
	{
		
	}

	int LilEddieGame::CompileGameDLL()
	{
		//Assumes we want game module/editor debug/release states to be the same (for now yes, later no)
		std::string config = "EditorGameRelease";
	#ifdef LIL_DEBUG
		config = "EditorGameDebug";
	#endif

		//Might need to change how we find output dir
		std::string outDir = gamePath + "bin/" + config + "/x64/";

		//<Mode> <solution> <OutputDir> <ProjName> <Config>
		std::string lilBuilder = "start /B ../Tools/Binaries/LilBuilder.exe gamemodule " + 
			slnPath + " " + 
			outDir + " " + 
			projName + " " + 
			config;

		return system(lilBuilder.c_str());
	}

	int LilEddieGame::ReloadGameDLL()
	{
		//Unload current scene (just use a new scene) (rn it loads it from file, not fast)
		std::string scenePath = "";
		if (sceneManager.scene)
			scenePath = sceneManager.scene->path;
		sceneManager.NewScene();

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
			return 1;
		}

		std::string versLn(128, 0);
		if (!fgets(&versLn[0], 128, versFile))
		{
			LIL_ERROR("LatestVersion.txt was empty.");
			return 2;
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
			return 3;
		}

		//Get address of CreateComponentFactory function
		void* createCompAddr = GetProcAddress(tempGameLib, "CreateComponentFactory");
		if (!createCompAddr)
		{
			LIL_ERROR("Could not find CreateComponentFactory function of game module, will not link new dll.");
			FreeLibrary(tempGameLib);
			return 4;
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
			return 5;
		}
		((void(*)(Game*))syncCorePtrs)(this);

		//Update game lib in current use
		if (gameLib)
			FreeLibrary((HMODULE)gameLib);
		gameLib = tempGameLib;

		//Load scene back
		if (scenePath != "")
			sceneManager.LoadScene(scenePath.c_str());

		return 0;
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
