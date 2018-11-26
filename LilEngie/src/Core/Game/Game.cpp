#include <iostream>
#include "Game.h"

namespace LilEngie
{
	Game::Game(std::function<void()> start, std::function<void()> update)
	{
		start();
		while (isRunning)
		{
			update();
		}
	}

	Game::~Game()
	{

	}
}
