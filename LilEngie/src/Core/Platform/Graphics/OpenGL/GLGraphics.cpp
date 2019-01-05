#ifdef LIL_WINDOWS
#include <windows.h>
#endif //LIL_WINDOWS

#include <fstream>
#include <sstream>
#include <string>
#include <GL/glew.h>
#include <Core/Platform/Window/Window.h>
#include <Core/Platform/Graphics/IGraphics.h>
#include <Core/Platform/Graphics/InputLayout.h>
#include <Core/Platform/Graphics/Handles.h>
#include <Core/Debug/Log.h>
#include <Core/Game/ServiceLocator.h>
#include "GLHandles.h"
#include "GLGraphics.h"

namespace LilEngie
{
	struct GLCtx
	{
	#ifdef LIL_WINDOWS
		HGLRC renderCtx;
		HDC devCtx;
	#endif // LIL_WINDOWS
	};

	GLGraphics::~GLGraphics()
	{ 
		if (ctx != nullptr)
		{
			ServiceLocator::Log()->Print(Verbosity::Warning, "OpenGL context destroyed incorrectly. Please use IGraphics::ReleaseGraphicsContext().");
			Shutdown();
		}
	}

	void GLGraphics::Init(const WinProp &windowProperties)
	{
		//Initialize glew
		if (!glewInit())
		{
			LIL(Log)->Print(Verbosity::Error, "Could not initialize opengl.");
			return;
		}

		ctx = new GLCtx();

	#ifdef LIL_WINDOWS
		ctx->devCtx = (HDC)windowProperties.hdc;
		ctx->renderCtx = wglCreateContext((HDC)windowProperties.hdc);
		SetContextCurrent();
	#endif //LIL_WINDOWS
	}

	void GLGraphics::SetClearColor(float r, float g, float b, float a)
	{
		clearColor[0] = r;
		clearColor[1] = g;
		clearColor[2] = b;
		clearColor[3] = a;
		glClearColor(r, g, b, a);
	}

	void GLGraphics::Clear()
	{
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	}

	//TODO
	void GLGraphics::Render()
	{

	}

	//TODO
	void GLGraphics::Resize(int width, int height)
	{

	}

	void GLGraphics::SetContextCurrent()
	{
	#ifdef LIL_WINDOWS
		wglMakeCurrent(ctx->devCtx, ctx->renderCtx);
	#endif //LIL_WINDOWS
	}

	IVertexShader* GLGraphics::CreateVertexShader(const std::string &file)
	{
		return CreateVertexShader(file, nullptr, nullptr, 0);
	}

	//TODO
	IVertexShader* GLGraphics::CreateVertexShader(const std::string &file, IInputLayout** layout, InputElement* elements, uint numElements)
	{
		/*
		//Create layout if necessary
		if (layout != nullptr)
			*layout = CreateLayout(elements, numElements);

		//File to string
		std::string src;
		std::ifstream t("file.txt");
		std::stringstream buffer;
		buffer << t.rdbuf();
		src = buffer.str();

		//Setup/Compile shader
		uint program = glCreateProgram();

		//Compile
		uint 
		*/
		return nullptr;
	}

	//TODO
	void GLGraphics::SetVertexShader(IVertexShader* shader)
	{
		
	}

	//TODO
	void GLGraphics::ReleaseVertexShader(IVertexShader** shader)
	{

	}

	//TODO
	IFragmentShader* GLGraphics::CreateFragmentShader(const std::string &file)
	{
		return nullptr;
	}

	//TODO
	void GLGraphics::SetFragmentShader(IFragmentShader* shader)
	{

	}

	//TODO
	void GLGraphics::ReleaseFragmentShader(IFragmentShader** shader)
	{

	}

	//TODO
	void GLGraphics::SetInputLayout(IInputLayout* shader)
	{

	}

	void GLGraphics::ReleaseInputLayout(IInputLayout** layout)
	{
		delete[] ((GLInputLayout*)layout)->elements;
		((GLInputLayout*)layout)->elements = nullptr;
		delete *layout;
		*layout = nullptr;
	}

	//TODO
	IVertexBuffer* GLGraphics::CreateVertexBuffer(float* verts, uint size)
	{
		return nullptr;
	}

	//TODO
	void GLGraphics::BindVertexBuffer(IVertexBuffer* vBuffer, uint stride)
	{
		
	}

	//TODO
	void GLGraphics::ReleaseVertexBuffer(IVertexBuffer** vBuffer)
	{
		
	}

	//TODO
	IIndexBuffer* GLGraphics::CreateIndexBuffer(uint* inds, uint size)
	{
		return nullptr;
	}

	//TODO
	void GLGraphics::BindIndexBuffer(IIndexBuffer* iBuffer)
	{
		
	}

	//TODO
	void GLGraphics::ReleaseIndexBuffer(IIndexBuffer** iBuffer)
	{
		
	}

	//TODO
	void GLGraphics::Draw(uint indexCount)
	{
		
	}

	void GLGraphics::Shutdown()
	{
	#ifdef LIL_WINDOWS
		wglDeleteContext(ctx->renderCtx);
	#endif //LIL_WINDOWS

		delete ctx;
		ctx = nullptr;
	}

	IInputLayout* GLGraphics::CreateLayout(InputElement* elements, uint numElements)
	{
		//Create new input layout
		GLInputLayout* layout = new GLInputLayout();

		//Copy element data
		layout->size = numElements;
		layout->elements = new InputElement[numElements];
		memcpy(layout->elements, elements, numElements * sizeof(InputElement));

		//Return
		return layout;
	}

	uint GLGraphics::CompileShader(const std::string &src, int type)
	{
		//Create shader with source
		uint shader = glCreateShader(type);
		const char* source = src.c_str();
		glShaderSource(shader, 1, &source, nullptr);

		//Compile
		glCompileShader(shader);

		//Error handling
		int result;
		glGetShaderiv(shader, GL_COMPILE_STATUS, &result);
		if (!result)
		{
			//Get length
			int len;
			glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &len);

			//Get message
			char* msg = new char[len];
			glGetShaderInfoLog(shader, len, &len, msg);

			//Output and clean
			LIL(Log)->Print(Verbosity::Error, "OpenGL shader compilation error.", msg);
			delete[] msg;
		}

		return shader;
	}
}
