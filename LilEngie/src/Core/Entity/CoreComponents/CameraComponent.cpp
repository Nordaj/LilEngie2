#include <math.h>
#include <Core/Math/LilMath.h>
#include <Core/Platform/Graphics/Handles.h>
#include <Core/Platform/Graphics/IGraphics.h>
#include <Core/Graphics/Renderer.h>
#include <Core/Debug/Log.h>
#include <Core/Entity/Actor.h>
#include <Core/Entity/CoreComponents/TransformComponent.h>
#include <Core/Debug/Log.h>
#include <Core/Game/Game.h>
#include "CameraComponent.h"

namespace LilEngie
{
	void CameraComponent::Init()
	{
		ResetProjection();

		//Subscribe to window resize
		Subscribe(EventType::WindowResize);

		renderer = &actor->game->renderer;

		renderer->cameras.push_back(&camera);

		if (useFramebuffer)
		{
			framebuffer = renderer->gfx->CreateFramebuffer(width, height);
			camera.framebuffer = framebuffer;
		}
	}

	void CameraComponent::OnDraw()
	{
		IGraphics* gfx = renderer->gfx;

		camera.view = inverse(actor->transform->GlobalTransformation());
		ResetProjection();

		camera.clearColor = clearColor;
	}

	void CameraComponent::UseFramebuffer()
	{
		//Only start using one if not already
		if (!useFramebuffer)
		{
			useFramebuffer = true;
			framebuffer = renderer->gfx->CreateFramebuffer(width, height);
			camera.framebuffer = framebuffer;
		}
	}

	void CameraComponent::ResizeFramebuffer(int width, int height)
	{
		if (framebuffer)
		{
			renderer->gfx->ReleaseFramebuffer(&framebuffer);
			framebuffer = renderer->gfx->CreateFramebuffer(width, height);
			camera.framebuffer = framebuffer;
			this->width = width;
			this->height = height;
		}
	}

	void CameraComponent::EnableDebugGraphics(bool enabled)
	{
		camera.renderDebug = enabled;
	}

	void CameraComponent::ResetProjection()
	{
		float ar = useFramebuffer ? width / (float)height : actor->game->renderer.aspectRatio;

		float t = camera.near * tan(fov / 2 * (PI / 180.f));
		float r = t * ar;
		camera.projection = Math::projection(r, -r, t, -t, camera.near, f);
	}
}
