#pragma once

#include <Core/Core.h>
#include <Core/Math/LilMath.h>

namespace LilEngie
{
	class Game;
	class Actor;
	class DebugDrawing;
	class CameraComponent;

	enum class TransformGizmoMode
	{
		Translate,
		Rotate,
		Scale
	};

	class LIL_API TransformGizmo
	{
	public:
		Actor* selected = nullptr;
		bool worldMode = true;
		TransformGizmoMode trsMode = TransformGizmoMode::Translate;

	private:
		Game* game;
		DebugDrawing* dbg;
		CameraComponent* camera;
		int selectedAxis = 0;
		vec3 axis;
		vec3 offset;

	public:
		void Init(Game* game, DebugDrawing* debugDrawing, CameraComponent* camera);
		void Draw(vec3 clipMousePos);

	private:
		void DrawTranslate(vec3 mp);
		void DrawRotate(vec3 mp);
		void DrawScale(vec3 mp);
	};
}
