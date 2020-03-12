#include <math.h>
#include <Core/Math/LilMath.h>
#include <Core/Platform/Graphics/Handles.h>
#include <Core/Platform/Graphics/IGraphics.h>
#include <Core/Graphics/Renderer.h>
#include <Core/Debug/Log.h>
#include <Core/Entity/Actor.h>
#include <Core/Entity/CoreComponents/TransformComponent.h>
#include "CameraComponent.h"

namespace LilEngie
{
	void CameraComponent::Start()
	{
		ResetProjection();

		//Subscribe to window resize
		Subscribe(EventType::WindowResize);
	}

	void CameraComponent::Update()
	{

	}

	void CameraComponent::OnDraw()
	{
		IGraphics* gfx = Renderer::core->gfx;

		//TODO: use the inverse in the future
		view = actor->transform->mat;

		mat4 vp = projection * view;

		ICBuffer* buf = Renderer::core->cbPerScene;
		void* loc = gfx->GetCBufferPtr(buf);
		memcpy(loc, &vp, sizeof(mat4));
		gfx->UpdateCBuffer(buf);
	}

	void CameraComponent::OnEvent(const Event& e)
	{
		if (e.type == EventType::WindowResize)
		{
			//Recalc aspect ratio
			int w = e.args[0].asInt;
			int h = e.args[1].asInt;
			ar = (float)w / (float)h;

			//Reset projection matrix
			ResetProjection();
		}
	}

	void CameraComponent::ResetProjection()
	{
		float t = n * tan(fov / 2 * (PI / 180.f));
		float r = t * ar;
		projection = Math::projection(r, -r, t, -t, n, f);
	}
}
