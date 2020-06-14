#pragma once

#include <Core/Core.h>
#include <Core/Math/LilMath.h>
#include <Core/Platform/Window/Keys.h>
#include <Core/Platform/Window/MouseState.h>

namespace LilEngie
{
	class Game;

	class LIL_API Input
	{
	public:
		static Input* core;
		
		bool* currentKeys;
		bool* prevKeys;

		MouseState prevMouse;
		MouseState* currentMouse;

		bool lockMousePos;
		bool mouseVisible = true;

	private:
		Game* game;

	public:
		Input();
		~Input();

		void Init(Game* game);

		bool GetKey(Key key);
		bool GetKeyDown(Key key);
		bool GetKeyUp(Key key);

		bool GetMouseBtn(MouseButton button);
		bool GetMouseBtnDown(MouseButton button);
		bool GetMouseBtnUp(MouseButton button);

		vec3 GetMousePos();
		vec3 GetMouseDelta();
		float GetWheelDelta();
		void MouseVisibility(bool visible);

		void Update();
	};
}
