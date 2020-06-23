#pragma once

#include <Core/Core.h>
#include <Core/Math/LilMath.h>
#include <Core/Graphics/Camera.h>
#include "../Actor.h"
#include "../IComponent.h"

namespace LilEngie
{
	class Renderer;
	class IFramebuffer;

	class LIL_API CameraComponent : public IComponent
	{
	public:
		float fov = 60;
		float n = .1f;
		float f = 100;

		bool useFramebuffer = false;
		IFramebuffer* framebuffer;
		int width, height;

		vec4 clearColor = vec4(.05, .05, .05, 1);

		mat4 view;
		mat4 projection;

		LIL_TYPE_NAME("camera")
		PROPERTIES(
			PROPERTY(fov)
			PROPERTY(n)
			PROPERTY(f)
			PROPERTY(clearColor)
			PROPERTY(useFramebuffer)
			PROPERTY(width)
			PROPERTY(height)
		)

	private:
		Camera camera;
		Renderer* renderer;

	public:
		void Start() override;
		void Update() override;
		void OnDraw() override;

	private:
		void ResetProjection();
	};
}
