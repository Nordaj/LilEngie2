#pragma once

#include <LilEngie.h>

namespace LilEddie
{
	class SceneCameraComponent : public IComponent
	{
	public:
		float lookSpeed = .01f;
		float moveSpeed = 5;

		LIL_TYPE_NAME("__EddieSceneCamera");
		PROPERTIES(
			PROPERTY(lookSpeed)
			PROPERTY(moveSpeed)
		)

	private:
		bool isLooking = false;

	public:
		void EditorUpdate() override;
	};
}
