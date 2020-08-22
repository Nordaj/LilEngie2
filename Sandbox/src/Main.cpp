#include <LilEngie.h>
#include "GameComponentFactory.h"

int main()
{
	Game* game = new Game();

	//Create our own component factory
	game->componentFactory = new GameComponentFactory();
	game->componentFactory->InitComponentList();
	ComponentFactory::core = game->componentFactory;

	LIL_LOG("Hello, welcome to Lil Engie 2.");

	game->Run();
	delete game;
	return 0;
}

//Component factory for game when using as dll (for LilEddie's use)
extern "C" __declspec(dllexport) ComponentFactory* CreateComponentFactory()
{
	return new GameComponentFactory();
}

//Sloppy solution to sync global core pointers across dll's
extern "C" __declspec(dllexport) void SyncGlobalPtrs(Game* game)
{
	game->SetCorePtrs();
}
