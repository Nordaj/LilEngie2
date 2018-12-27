#include <Core/Debug/Log.h>
#include <Core/Game/ServiceLocator.h>
#include <Core/Platform/Graphics/IGraphics.h>
#include "Renderer.h"

namespace LilEngie
{
	IInputLayout* layout;
	IVertexShader* vert;
	IFragmentShader* frag;
	IVertexBuffer* vBuffer;
	IIndexBuffer* iBuffer;

	Renderer::Renderer()
	{

	}

	Renderer::~Renderer()
	{
		if (gfx != nullptr)
			Shutdown();
	}

	void Renderer::Init(const WinProp &windowProperties, GraphicsAPI api)
	{
		gfx = IGraphics::CreateGraphicsContext(api);
		gfx->Init(windowProperties);
		gfx->SetClearColor(clearColor[0], clearColor[1], clearColor[2], clearColor[3]);
		
		Subscribe(EventType::WindowResize);

		//Shader and layout creation
		InputElement elements[1] = {
			InputElement("POSITION", InputFormat::FLOAT_R32G32B32, 0)
		};

		vert = gfx->CreateVertexShader("LilEngie/res/Shaders/UnlitVS.hlsl", &layout, elements, 1);
		frag = gfx->CreateFragmentShader("LilEngie/res/Shaders/UnlitFS.hlsl");

		//Model creation
		float verts[] = {
			-0.5f, -0.5f, 0,
			0.0f,  0.5f, 0,
			0.5f, -0.5f, 0
		};
		uint inds[] = { 0, 1, 2 };

		vBuffer = gfx->CreateVertexBuffer(verts, sizeof(float) * 9);
		iBuffer = gfx->CreateIndexBuffer(inds, sizeof(uint) * 3);

		//Draw prep
		gfx->SetInputLayout(layout);
		gfx->SetVertexShader(vert);
		gfx->SetFragmentShader(frag);

		gfx->BindVertexBuffer(vBuffer, sizeof(float) * 3);
		gfx->BindIndexBuffer(iBuffer);
	}

	void Renderer::Shutdown()
	{
		//Cleanup
		gfx->ReleaseInputLayout(&layout);
		gfx->ReleaseVertexShader(&vert);
		gfx->ReleaseFragmentShader(&frag);

		gfx->ReleaseVertexBuffer(&vBuffer);
		gfx->ReleaseIndexBuffer(&iBuffer);

		IGraphics::ShutdownGraphicsContext(&gfx);
	}

	void Renderer::SetClearColor(float r, float g, float b, float a)
	{
		clearColor[0] = r;
		clearColor[1] = g;
		clearColor[2] = b;
		clearColor[3] = a;
		gfx->SetClearColor(r, g, b, a);
	}

	void Renderer::Render()
	{
		gfx->Clear();

		//RENDER
		//Draw
		gfx->Draw(3);

		gfx->Render();
	}

	void Renderer::OnEvent(const Event &e)
	{
		switch (e.type)
		{
			case EventType::WindowResize:
				gfx->Resize(e.args[0].asInt, e.args[1].asInt);
				break;
			default:
				break;
		}
	}
}
