#include <Core/Platform/Graphics/IGraphics.h>
#include <Core/Debug/Log.h>
#include <Core/Graphics/Vertex.h>
#include "Renderer.h"
#include "Mesh.h"

namespace LilEngie
{
	Mesh::Mesh()
		: vertexBuffer(nullptr), indexBuffer(nullptr), indexCount(0)
	{ }

	Mesh::~Mesh()
	{ 
		Renderer::core->gfx->ReleaseVertexBuffer(&vertexBuffer);
		Renderer::core->gfx->ReleaseIndexBuffer(&indexBuffer);
	}

	bool Mesh::Init(const std::vector<Vertex>& vertices, const std::vector<uint>& indices)
	{
		//Make sure not currently init
		if (IsInit())
			return false;

		//Create bufers
		vertexBuffer = Renderer::core->gfx->CreateVertexBuffer((float*)&vertices[0], sizeof(Vertex) * vertices.size());
		indexCount = indices.size();
		indexBuffer = Renderer::core->gfx->CreateIndexBuffer((uint*)&indices[0], sizeof(uint) * indexCount);

		//Make sure it succeeded
		if (!vertexBuffer || !indexBuffer)
		{
			LIL_ERROR("Could not create vertex or index buffer for mesh.");
			vertexBuffer = nullptr;
			indexBuffer = nullptr;
			return false;
		}
		return true;
	}

	bool Mesh::IsInit()
	{
		return indexCount > 0;
	}

	void Mesh::Render(IGraphics* gfxContext)
	{
		gfxContext->BindVertexBuffer(vertexBuffer, sizeof(Vertex));
		gfxContext->BindIndexBuffer(indexBuffer);
		gfxContext->Draw(indexCount);
	}
}
