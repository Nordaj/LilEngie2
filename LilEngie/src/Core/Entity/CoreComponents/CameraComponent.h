#pragma once

#include <Core/Core.h>
#include <Core/Math/LilMath.h>
#include "../Actor.h"
#include "../IComponent.h"

namespace LilEngie
{
	class LIL_API CameraComponent : public IComponent
	{
	public:
		float fov = 60;
		float n = .1f;
		float f = 100;
		float ar = 16.f / 9.f;

		mat4 view;
		mat4 projection;

		PROPERTIES(
			PROPERTY(fov)
			PROPERTY(n)
			PROPERTY(f)
			PROPERTY(ar)
		)

	public:
		void Start() override;
		void Update() override;
		void OnDraw() override;

		void OnEvent(const Event& e) override;

	private:
		void ResetProjection();
	};
}
