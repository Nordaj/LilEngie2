#include <Core/Platform/Graphics/IGraphics.h>
#include <Core/Debug/Log.h>
#include <Core/Debug/DebugTimer.h>
#include <Core/Resources/ResourceManager.h>
#include <Core/Resources/Types/MeshResource.h>
#include <Core/Math/LilMath.h>
#include <Core/Graphics/Mesh.h>
#include "Renderer.h"

namespace LilEngie
{
	Renderer* Renderer::core = nullptr;

	IInputLayout* layout;
	IShader* shader;
	//MeshResource* meshResource;
	ICBuffer* colorBuffer;
	ICBuffer* modMatBuffer;

	Renderer::Renderer()
	{
		opaqueQueue = std::queue<Mesh*>();
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
		InputElement elements[4] = {
			InputElement("POSITION", InputFormat::FLOAT_R32G32B32, 0),
			InputElement("NORMAL", InputFormat::FLOAT_R32G32B32, sizeof(float) * 3),
			InputElement("TANGENT", InputFormat::FLOAT_R32G32B32, sizeof(float) * 6),
			InputElement("TEXCOORD", InputFormat::FLOAT_R32G32, sizeof(float) * 9)
		};

		shader = gfx->CreateShader("LilEngie/res/Shaders/UnlitVS", "LilEngie/res/Shaders/UnlitFS", &layout, elements, 4);

		//Model matrix
		mat4 model = mat4(1);
		scale(model, vec3(1, 1, 1));
		rotate(model, vec3(0, 0, 3.14f / 2));
		translate(model, vec3(0, 0, 0));

		//Constant buffer creation
		colorBuffer = gfx->CreateCBuffer(sizeof(float) * 4);
		void* colLoc = gfx->GetCBufferPtr(colorBuffer);
		float myColor[4] = { 0.5f, 1, 1, 1 };
		memcpy(colLoc, &myColor, sizeof(float) * 4);
		gfx->UpdateCBuffer(colorBuffer);

		modMatBuffer = gfx->CreateCBuffer(sizeof(mat4));
		void* modLoc = gfx->GetCBufferPtr(modMatBuffer);
		memcpy(modLoc, &model, sizeof(mat4));
		gfx->UpdateCBuffer(modMatBuffer);

		//Create mesh here
		//std::string path("LilEngie/res/Models/teapot.fbx");
		//ResourceId id = ResourceId(path, ResourceType::Mesh);
		//meshResource = (MeshResource*)ResourceManager::core->LoadResource(id);

		gfx->SetInputLayout(layout);
		gfx->SetShader(shader);

		gfx->BindCBuffer(colorBuffer, ShaderType::Fragment, 0);
		gfx->BindCBuffer(modMatBuffer, ShaderType::Vertex, 1);
	}

	void Renderer::Shutdown()
	{
		//Cleanup
		gfx->ReleaseInputLayout(&layout);
		gfx->ReleaseShader(&shader);

		gfx->ReleaseCBuffer(&colorBuffer);
		gfx->ReleaseCBuffer(&modMatBuffer);

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

	void Renderer::QueueOpaque(Mesh* mesh)
	{
		opaqueQueue.push(mesh);
	}

	void Renderer::Render()
	{
		gfx->Clear();

		//Draw Meshes
		while (opaqueQueue.size() > 0)
		{
			opaqueQueue.back()->Render();
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
