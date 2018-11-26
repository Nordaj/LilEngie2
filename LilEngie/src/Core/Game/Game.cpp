#include <iostream>
#include "Game.h"

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
