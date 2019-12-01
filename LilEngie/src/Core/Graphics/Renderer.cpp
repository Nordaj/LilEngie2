#include <Core/Debug/Log.h>
#include <Core/Platform/Graphics/IGraphics.h>
#include <Core/Debug/Log.h>
#include "Renderer.h"

namespace LilEngie
{
	IInputLayout* layout;
	IShader* shader;
	IVertexBuffer* vBuffer;
	IIndexBuffer* iBuffer;
	ICBuffer* colorBuffer;

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
		//Log which api used
		switch (api)
		{
			case GraphicsAPI::DirectX11:
				LIL_LOG(Verbosity::Verbose, "Initializing DirectX11");
				break;
			case GraphicsAPI::OpenGL:
				LIL_LOG(Verbosity::Verbose, "Initializing OpenGL");
				break;
			default:
				break;
		}

		//Initialize graphics
		gfx = IGraphics::CreateGraphicsContext(api);
		gfx->Init(windowProperties);
		gfx->SetClearColor(clearColor[0], clearColor[1], clearColor[2], clearColor[3]);
		
		Subscribe(EventType::WindowResize);

		//Shader and layout creation
		InputElement elements[2] = {
			InputElement("POSITION", InputFormat::FLOAT_R32G32B32, 0),
			InputElement("TEXCOORD", InputFormat::FLOAT_R32G32, sizeof(float) * 3)
		};

		shader = gfx->CreateShader("LilEngie/res/Shaders/UnlitVS", "LilEngie/res/Shaders/UnlitFS", &layout, elements, 2);

		//Constant buffer creation
		colorBuffer = gfx->CreateCBuffer(sizeof(float) * 4);
		void* c = gfx->GetCBufferPtr(colorBuffer);
		float myColor[4] = { 0.5f, 1, 1, 1 };
		memcpy(c, &myColor, sizeof(float) * 4);
		gfx->UpdateCBuffer(colorBuffer);

		//Model creation
		float verts[] = {
			//POSITION			//TEXCOORD
			-0.5f, -0.5f, 0,	0.0f, 1.0f, //Bottom-Left
			 0.0f,  0.5f, 0,	0.5f, 0.0f, //Top
			 0.5f, -0.5f, 0,	1.0f, 1.0f  //Bottom-Right
		};
		uint inds[] = { 0, 1, 2 };

		vBuffer = gfx->CreateVertexBuffer(verts, sizeof(float) * 15);
		iBuffer = gfx->CreateIndexBuffer(inds, sizeof(uint) * 3);

		//Draw prep
		gfx->BindVertexBuffer(vBuffer, sizeof(float) * 5);
		gfx->BindIndexBuffer(iBuffer);

		gfx->SetInputLayout(layout);
		gfx->SetShader(shader);

		gfx->BindCBuffer(colorBuffer, ShaderType::Fragment, 0);
	}

	void Renderer::Shutdown()
	{
		//Cleanup
		gfx->ReleaseInputLayout(&layout);
		gfx->ReleaseShader(&shader);

		gfx->ReleaseVertexBuffer(&vBuffer);
		gfx->ReleaseIndexBuffer(&iBuffer);

		gfx->ReleaseCBuffer(&colorBuffer);

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
