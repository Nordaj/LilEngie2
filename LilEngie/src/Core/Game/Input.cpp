#include <memory>
#include "Game.h"
#include "Input.h"

namespace LilEngie
{
	Input* Input::core = nullptr;

	Input::Input()
	{
		core = core ? core : this;
	}

	void Input::Init(Game* game)
	{
		this->game = game;
		currentKeys = game->application.GetKeyStates();
		prevKeys = new bool[255];
	}

	Input::~Input()
	{
		delete[] prevKeys;
	}

	bool Input::GetKey(Key key)
	{
		return currentKeys[(uint8)key];
	}

	bool Input::GetKeyDown(Key key)
	{
		return currentKeys[(uint8)key] && !prevKeys[(uint8)key];
	}

	bool Input::GetKeyUp(Key key)
	{
		return !currentKeys[(uint8)key] && prevKeys[(uint8)key];
	}

	void Input::CacheKeyStates()
	{
		memcpy(prevKeys, currentKeys, 255);
	}
}
