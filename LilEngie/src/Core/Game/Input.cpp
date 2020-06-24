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
		currentKeys = game->application.window.GetKeyStates();
		currentMouse = game->application.window.GetMouseState();
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

	void Input::Update()
	{
		//Cache prev input data
		memcpy(prevKeys, currentKeys, 255);
		memcpy(&prevMouse, currentMouse, sizeof(MouseState));

		//Resets mouse deltas each frame
		currentMouse->rawDeltaX = 0;
		currentMouse->rawDeltaY = 0;
		currentMouse->wheelDelta = 0;

		//Lock mouse position to center of the screen
		if (lockMousePos)
		{
			int x, y;
			game->application.window.GetWindowPos(&x, &y);

			x += game->application.windowProperties.width / 2;
			y += game->application.windowProperties.height / 2;

			game->application.window.GetMousePosition(&currentMouse->mouseX, &currentMouse->mouseY);
			game->application.window.SetMousePosition(x, y);

			prevMouse.mouseX = x;
			prevMouse.mouseY = y;

			//Make sure there are no jumps in delta during transition to locked mouse
			if (!wasLocked)
			{
				wasLocked = true;
				currentMouse->mouseX = x;
				currentMouse->mouseY = y;
			}
		}
		else
			wasLocked = false;
	}

	bool Input::GetMouseBtn(MouseButton button)
	{
		return currentMouse->buttons[(uint)button];
	}

	bool Input::GetMouseBtnDown(MouseButton button)
	{
		return currentMouse->buttons[(uint)button] && !prevMouse.buttons[(uint)button];
	}

	bool Input::GetMouseBtnUp(MouseButton button)
	{
		return !currentMouse->buttons[(uint)button] && prevMouse.buttons[(uint)button];
	}

	vec3 Input::GetMousePos()
	{
		return vec3(currentMouse->mouseX, currentMouse->mouseY, 0);
	}

	vec3 Input::GetMouseDelta()
	{
		int deltaX = currentMouse->mouseX - prevMouse.mouseX;
		int deltaY = currentMouse->mouseY - prevMouse.mouseY;
		return vec3(deltaX, deltaY, currentMouse->wheelDelta);
	}

	float Input::GetWheelDelta()
	{
		return currentMouse->wheelDelta;
	}

	void Input::MouseVisibility(bool visible)
	{
		game->application.window.SetMouseVisibility(visible);
		mouseVisible = visible;
	}
}
