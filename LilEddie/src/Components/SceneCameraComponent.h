#pragma once

#include <LilEngie.h>

namespace LilEddie
{
	class SceneCameraComponent : public IComponent
	{
	public:
		float lookSpeed = .01f;
		float moveSpeed = 5;
		bool hovered = false;

		LIL_TYPE_NAME("__EddieSceneCamera");
		PROPERTIES(
			PROPERTY(lookSpeed)
			PROPERTY(moveSpeed)
		)

	private:
		vec3 euler;
		bool isLooking = false;
		DebugDrawing* dbg;

	public:
		void Init() override;
		void OnDraw() override;
		void EditorUpdate() override;

	private:
		void DrawSceneLines();
	};
}
