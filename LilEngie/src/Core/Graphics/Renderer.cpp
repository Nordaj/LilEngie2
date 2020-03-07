#include <Core/Platform/Graphics/IGraphics.h>
#include <Core/Debug/Log.h>
#include <Core/Debug/DebugTimer.h>
#include <Core/Resources/ResourceManager.h>
#include <Core/Resources/Types/MeshResource.h>
#include <Core/Math/LilMath.h>
#include "IRenderable.h"
#include "Renderer.h"

namespace LilEngie
{
	Renderer* Renderer::core = nullptr;

	IInputLayout* layout;
	IShader* shader;

	Renderer::Renderer()
	{
		opaqueQueue = std::queue<IRenderable*>();
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

		//Create defualt constant buffers
		cbPerObject = gfx->CreateCBuffer(sizeof(mat4));
		cbPerScene = gfx->CreateCBuffer(sizeof(mat4));

		//Shader and layout creation
		InputElement elements[4] = {
			InputElement("POSITION", InputFormat::FLOAT_R32G32B32A32, 0),
			InputElement("NORMAL", InputFormat::FLOAT_R32G32B32A32, sizeof(float) * 4),
			InputElement("TANGENT", InputFormat::FLOAT_R32G32B32A32, sizeof(float) * 8),
			InputElement("TEXCOORD", InputFormat::FLOAT_R32G32B32A32, sizeof(float) * 12)
		};

		shader = gfx->CreateShader("LilEngie/res/Shaders/UnlitVS", "LilEngie/res/Shaders/UnlitFS", &layout, elements, 4);
	}

	void Renderer::Shutdown()
	{
		//Cleanup
		gfx->ReleaseInputLayout(&layout);
		gfx->ReleaseShader(&shader);

		gfx->ReleaseCBuffer(&cbPerObject);
		gfx->ReleaseCBuffer(&cbPerScene);

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

	void Renderer::QueueOpaque(IRenderable* renderable)
	{
		opaqueQueue.push(renderable);
	}

	void Renderer::Render()
	{
		gfx->Clear();

		//Constant buffer management
		gfx->BindCBuffer(cbPerObject, ShaderType::Vertex, 1);
		gfx->BindCBuffer(cbPerScene, ShaderType::Vertex, 2);

		//Setup default shader and input layout
		gfx->SetInputLayout(layout);
		gfx->SetShader(shader);

		//Draw Meshes
		while (opaqueQueue.size() > 0)
		{
			opaqueQueue.back()->Render(gfx);
			opaqueQueue.pop();
		}

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
