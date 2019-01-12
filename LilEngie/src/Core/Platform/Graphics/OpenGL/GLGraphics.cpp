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
		//Context creation
		ctx = new GLCtx();
	#ifdef LIL_WINDOWS
		ctx->devCtx = (HDC)windowProperties.hdc;
		ctx->renderCtx = wglCreateContext((HDC)windowProperties.hdc);
		SetContextCurrent();
	#endif //LIL_WINDOWS

		//Initialize glew
		if (glewInit() != GLEW_OK)
		{
			LIL(Log)->Print(Verbosity::Error, "Could not initialize OpenGL.");
			return;
		}
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

	void GLGraphics::Render()
	{
		//Doesn't need anything
	}

	void GLGraphics::Resize(int width, int height)
	{
		glViewport(0, 0, width, height);
	}

	void GLGraphics::SetContextCurrent()
	{
	#ifdef LIL_WINDOWS
		wglMakeCurrent(ctx->devCtx, ctx->renderCtx);
	#endif //LIL_WINDOWS
	}

	IShader* GLGraphics::CreateShader(const std::string &vert, const std::string &frag)
	{
		return CreateShader(vert, frag, nullptr, nullptr, 0);
	}

	IShader* GLGraphics::CreateShader(const std::string &vert, const std::string &frag, IInputLayout** layout, InputElement* elements, uint numElements)
	{
		//Create layout if necessary
		if (layout != nullptr)
			*layout = CreateLayout(elements, numElements);

		//Add extension if not yet added
		std::string vFile;
		if (vert.substr(vert.size() - 5, 5) != ".hlsl")
			vFile = vert + ".hlsl";
		else
			vFile = vert;

		std::string fFile;
		if (frag.substr(frag.size() - 5, 5) != ".hlsl")
			fFile = frag + ".hlsl";
		else
			fFile = frag;

		//Files to strings
		std::stringstream vertBuf;
		vertBuf << std::ifstream(vFile).rdbuf();
		std::string vertSrc = vertBuf.str();

		std::stringstream fragBuf;
		fragBuf << std::ifstream(fFile).rdbuf();
		std::string fragSrc = fragBuf.str();

		//Setup/Compile shader
		uint program = glCreateProgram();

		//Compile
		uint vertShader = CompileShader(vertSrc, GL_VERTEX_SHADER);
		uint fragShader = CompileShader(fragSrc, GL_FRAGMENT_SHADER);

		//Setup program
		glAttachShader(program, vertShader);
		glAttachShader(program, fragShader);
		glLinkProgram(program);
		glValidateProgram(program);

		//Clean
		glDeleteShader(vertShader);
		glDeleteShader(fragShader);

		GLShader* shader = new GLShader();
		shader->shader = program;
		return shader;
	}

	void GLGraphics::SetShader(IShader* shader)
	{
		glUseProgram(((GLShader*)shader)->shader);
	}

	void GLGraphics::ReleaseShader(IShader** shader)
	{
		glDeleteProgram(((GLShader*)*shader)->shader);
		((GLShader*)*shader)->shader = 0;
		delete *shader;
		*shader = nullptr;
	}

	void GLGraphics::SetInputLayout(IInputLayout* layout)
	{
		//Gets the opengl type from the format
		auto GetGLType = [](InputFormat ie) -> uint
		{
			switch (ie)
			{
				case LilEngie::FLOAT_R32G32B32A32:
					return GL_FLOAT;
					break;
				case LilEngie::FLOAT_R32G32B32:
					return GL_FLOAT;
					break;
				case LilEngie::FLOAT_R32G32:
					return GL_FLOAT;
					break;
				case LilEngie::FLOAT_R32:
					return GL_FLOAT;
					break;
				case LilEngie::UINT_R32:
					return GL_UNSIGNED_INT;
					break;
				default:
					return 0;
					break;
			}
		};

		//Unnecessary conversion for simplicity
		GLInputLayout* l = (GLInputLayout*)layout;

		//Set each attrib
		for (int i = 0; i < l->size; i++)
		{
			glEnableVertexAttribArray(i); 
			glVertexAttribPointer(i, l->elements[i].GetCount(), GetGLType(l->elements[i].format), false, 0, (void*)l->elements[i].offset);
			glVertexAttribPointer(0, 3, GL_FLOAT, false, 0, (void*)0);
		}
	}

	void GLGraphics::ReleaseInputLayout(IInputLayout** layout)
	{
		delete[] ((GLInputLayout*)*layout)->elements;
		((GLInputLayout*)*layout)->elements = nullptr;
		delete *layout;
		*layout = nullptr;
	}

	IVertexBuffer* GLGraphics::CreateVertexBuffer(float* verts, uint size)
	{
		GLVertexBuffer* vbo = new GLVertexBuffer();
		glGenBuffers(1, &(vbo->buffer));
		glBindBuffer(GL_ARRAY_BUFFER, vbo->buffer);
		glBufferData(GL_ARRAY_BUFFER, size, verts, GL_STATIC_DRAW);
		return vbo;
	}

	void GLGraphics::BindVertexBuffer(IVertexBuffer* vBuffer, uint stride)
	{
		glBindBuffer(GL_ARRAY_BUFFER, ((GLVertexBuffer*)vBuffer)->buffer);
	}

	void GLGraphics::ReleaseVertexBuffer(IVertexBuffer** vBuffer)
	{
		glDeleteBuffers(1, &((GLVertexBuffer*)*vBuffer)->buffer);
		((GLVertexBuffer*)*vBuffer)->buffer = 0;
		delete *vBuffer;
		*vBuffer = nullptr;
	}

	IIndexBuffer* GLGraphics::CreateIndexBuffer(uint* inds, uint size)
	{
		GLIndexBuffer* ibo = new GLIndexBuffer();
		glGenBuffers(1, &(ibo->buffer));
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo->buffer);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, size, inds, GL_STATIC_DRAW);
		return ibo;
	}

	void GLGraphics::BindIndexBuffer(IIndexBuffer* iBuffer)
	{
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ((GLIndexBuffer*)iBuffer)->buffer);
	}

	void GLGraphics::ReleaseIndexBuffer(IIndexBuffer** iBuffer)
	{
		glDeleteBuffers(1, &((GLIndexBuffer*)*iBuffer)->buffer);
		((GLIndexBuffer*)*iBuffer)->buffer = 0;
		delete *iBuffer;
		*iBuffer = nullptr;
	}

	void GLGraphics::Draw(uint indexCount)
	{
		glDrawElements(GL_TRIANGLES, indexCount, GL_UNSIGNED_INT, NULL);
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