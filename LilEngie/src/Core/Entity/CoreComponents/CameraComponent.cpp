#include <Core/Math/LilMath.h>
#include <Core/Platform/Graphics/Handles.h>
#include <Core/Platform/Graphics/IGraphics.h>
#include <Core/Graphics/Renderer.h>
#include "CameraComponent.h"

namespace LilEngie
{
	void CameraComponent::Update()
	{

	}

	void CameraComponent::OnDraw()
	{
		IGraphics* gfx = Renderer::core->gfx;

		translate(view, vec3(.01f, .01f, 0) * -1);

		mat4 vp = projection * view;

		ICBuffer* buf = Renderer::core->cbPerScene;
		void* loc = gfx->GetCBufferPtr(buf);
		memcpy(loc, &vp, sizeof(mat4));
		gfx->UpdateCBuffer(buf);
	}
}
