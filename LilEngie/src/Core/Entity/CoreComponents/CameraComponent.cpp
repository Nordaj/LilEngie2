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
	void CameraComponent::Start()
	{
		ResetProjection();

		//Subscribe to window resize
		Subscribe(EventType::WindowResize);

		renderer = &actor->game->renderer;

		renderer->cameras.push_back(&camera);
	}

	void CameraComponent::Update()
	{

	}

	void CameraComponent::OnDraw()
	{
		IGraphics* gfx = renderer->gfx;

		view = inverse(actor->transform->GlobalTransformation());
		ResetProjection();
		camera.vp = projection * view;

		camera.clearColor = clearColor;
	}

	void CameraComponent::ResetProjection()
	{
		float t = n * tan(fov / 2 * (PI / 180.f));
		float r = t * actor->game->renderer.aspectRatio;
		projection = Math::projection(r, -r, t, -t, n, f);
	}
}
