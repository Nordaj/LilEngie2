#include <Core/Platform/Graphics/IGraphics.h>
#include "Renderer.h"
#include "Mesh.h"
#include "MeshRenderer.h"

namespace LilEngie
{
	void MeshRenderer::Render(IGraphics* gfx)
	{
		//Update transform constant buffer
		ICBuffer* cb = Renderer::core->transformBuffer;
		memcpy(gfx->GetCBufferPtr(cb), &transform, sizeof(mat4));
		gfx->UpdateCBuffer(cb);

		//Render
		mesh->Render(gfx);
	}
}
