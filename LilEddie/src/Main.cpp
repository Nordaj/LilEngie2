//LilEddie - LilEngie's Editor application entry point

#include <iostream>
#include "LilEddieGame.h"

int main()
{
	LilEddie::LilEddieGame* game = new LilEddie::LilEddieGame();
	game->Run();
	delete game;

	return 0;
}
