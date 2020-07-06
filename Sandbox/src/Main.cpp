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
