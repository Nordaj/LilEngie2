#define STB_IMAGE_IMPLEMENTATION
#include <stb/stb_image.h>
#include <Vendor/imgui/imgui.h>
#include <Core/Platform/Graphics/IGraphics.h>
#include <Core/Debug/Log.h>
#include <Core/Debug/DebugTimer.h>
#include <Core/Resources/ResourceManager.h>
#include <Core/Resources/Types/MeshResource.h>
#include <Core/Graphics/Material.h>
#include <Core/Math/LilMath.h>
#include <Core/Application/Application.h>
#include <Core/Game/Game.h>
#include "IRenderable.h"
#include "Renderer.h"

namespace LilEngie
{
	IInputLayout* layout;
	IShader* shader;
	ITexture* texture;

	Renderer::Renderer()
	{
		opaqueQueue = std::vector<IRenderable*>();
	}

	Renderer::~Renderer()
	{
		if (gfx != nullptr)
			Shutdown();
	}

	void Renderer::Init(const WinProp& windowProperties, GraphicsAPI api, Game* game)
	{
		this->game = game;

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
		InitImGui(windowProperties);
		gfx->SetClearColor(clearColor[0], clearColor[1], clearColor[2], clearColor[3]);

		aspectRatio = windowProperties.width / (float)windowProperties.height;

		Subscribe(EventType::WindowResize);
		Subscribe(EventType::WindowClose);

		//Create defualt constant buffers
		cbPerObject = gfx->CreateCBuffer(sizeof(mat4));
		cbPerCamera = gfx->CreateCBuffer(sizeof(mat4));

		//Shader and layout creation
		InputElement elements[4] = {
			InputElement("POSITION", InputFormat::FLOAT_R32G32B32A32, 0),
			InputElement("NORMAL", InputFormat::FLOAT_R32G32B32A32, sizeof(float) * 4),
			InputElement("TANGENT", InputFormat::FLOAT_R32G32B32A32, sizeof(float) * 8),
			InputElement("TEXCOORD", InputFormat::FLOAT_R32G32B32A32, sizeof(float) * 12)
		};

		std::string shaderPath = game->gamePath + "res/Shaders/Unlit";
		shader = gfx->CreateShader(shaderPath + "VS", shaderPath + "FS", &layout, elements, 4);
	}

	void Renderer::Shutdown()
	{
		//Cleanup
		gfx->ImGuiShutdown();

		gfx->ReleaseInputLayout(&layout);
		gfx->ReleaseShader(&shader);

		gfx->ReleaseCBuffer(&cbPerObject);
		gfx->ReleaseCBuffer(&cbPerCamera);

		if (framebuffer)
			gfx->ReleaseFramebuffer(&framebuffer);

		IGraphics::ShutdownGraphicsContext(&gfx);
	}

	void Renderer::UseFramebuffer()
	{
		if (framebuffer)
			return;

		int w = game->application.windowProperties.width;
		int h = game->application.windowProperties.height;
		framebuffer = gfx->CreateFramebuffer(w, h);
	}

	void Renderer::ResizeFramebuffer(int w, int h)
	{
		if (!framebuffer)
			return;

		gfx->ReleaseFramebuffer(&framebuffer);
		framebuffer = gfx->CreateFramebuffer(w, h);
		aspectRatio = w / (float)h;
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
		opaqueQueue.push_back(renderable);
	}

	void Renderer::Render()
	{
		//Dont render if going to close
		if (isClosing) return;

		//Constant buffer management
		gfx->BindCBuffer(cbPerCamera, ShaderType::Vertex, 1);
		gfx->BindCBuffer(cbPerObject, ShaderType::Vertex, 2);

		//Setup default shader and input layout
		gfx->SetInputLayout(layout);
		gfx->SetShader(shader);

		//Clear window
		gfx->SetClearColor(clearColor[0], clearColor[1], clearColor[2], clearColor[3]);
		gfx->Clear();

		//Render the camers after setup
		RenderAllCameras();

		//Render ImGui (maybe disable when no imgui)
		gfx->UnbindFramebuffer();

		gfx->ImGuiRender();
		gfx->ImGuiNewFrame();


		gfx->Render();
	}

	void Renderer::OnEvent(const Event &e)
	{
		switch (e.type)
		{
			case EventType::WindowResize:
				gfx->Resize(e.args[0].asInt, e.args[1].asInt);
				if (!framebuffer)
					aspectRatio = e.args[0].asInt / (float)e.args[1].asInt;
				break;
			case EventType::WindowClose:
				isClosing = true;
				break;
			default:
				break;
		}
	}

	void Renderer::RenderAllCameras()
	{
		for (Camera* c : cameras)
		{
			//Bind framebuffer if present, otherwise just render to screen
			if (c->framebuffer)
				gfx->BindFramebuffer(c->framebuffer);
			else if (framebuffer)
				gfx->BindFramebuffer(framebuffer);
			else
				gfx->UnbindFramebuffer();

			//Clear
			gfx->SetClearColor(c->clearColor.r, c->clearColor.g, c->clearColor.b, c->clearColor.a);
			gfx->Clear();

			//Update camera cbuffer
			void* loc = gfx->GetCBufferPtr(cbPerCamera);
			memcpy(loc, &c->vp, sizeof(mat4));
			gfx->UpdateCBuffer(cbPerCamera);

			//Render opaque geometry (only works once: TODO fix)
			for (IRenderable* r : opaqueQueue)
				r->Render(gfx);
		}

		opaqueQueue.clear();
	}

	void Renderer::InitImGui(const WinProp& windowProperties)
	{
		// Setup Dear ImGui context
		IMGUI_CHECKVERSION();
		ImGui::CreateContext();
		ImGuiIO& io = ImGui::GetIO(); (void)io;

		//Kind of editor only 
		io.ConfigFlags |= ImGuiConfigFlags_DockingEnable | ImGuiConfigFlags_ViewportsEnable;

		// Setup Dear ImGui style
		ImGui::StyleColorsDark();
		//ImGui::StyleColorsClassic();

		//Init imgui
		gfx->ImGuiInit(windowProperties);

		//Automatically start first frame
		gfx->ImGuiNewFrame();
	}
}
