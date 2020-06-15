#ifdef LIL_WINDOWS
#include <windows.h>
#endif //LIL_WINDOWS

#include <fstream>
#include <sstream>
#include <string>
#include <GL/glew.h>
#include <Vendor/imgui/imgui.h>
#include <Vendor/imgui/imgui_impl_win32.h>
#include <Vendor/imgui/imgui_impl_opengl3.h>
#include <Core/Platform/Window/Window.h>
#include <Core/Platform/Graphics/IGraphics.h>
#include <Core/Platform/Graphics/InputLayout.h>
#include <Core/Platform/Graphics/Handles.h>
#include <Core/Debug/Log.h>
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

	void GLAPIENTRY MessageCallback(GLenum source, GLenum type, GLuint id, GLenum severity, GLsizei length, const GLchar* message, const void* userParam)
	{
		if (type == GL_DEBUG_TYPE_ERROR)
			LIL_ERROR("GL_ERROR: %s type = 0x%x, severity = 0x%x, message = %s\n", type, severity, message);
	}

	GLGraphics::~GLGraphics()
	{ 
		if (ctx != nullptr)
		{
			LIL_WARN("OpenGL context destroyed incorrectly. Please use IGraphics::ReleaseGraphicsContext().");
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
			LIL_ERROR("Could not initialize OpenGL.");
			return;
		}

		glEnable(GL_DEPTH_TEST);

		glFrontFace(GL_CW);
		glCullFace(GL_BACK);
		glEnable(GL_CULL_FACE);

	#ifdef LIL_DEBUG
		glEnable(GL_DEBUG_OUTPUT);
		glDebugMessageCallback(MessageCallback, 0);
	#endif //LIL_DEBUG
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
		if (vert.substr(vert.size() - 5, 5) != ".glsl")
			vFile = vert + ".glsl";
		else
			vFile = vert;

		std::string fFile;
		if (frag.substr(frag.size() - 5, 5) != ".glsl")
			fFile = frag + ".glsl";
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

		//Unnecessary but easier to read like this
		GLInputLayout* l = (GLInputLayout*)layout;

		//Get stride
		uint stride = 0;
		for (int i = 0; i < l->size; i++)
			stride += l->elements[i].GetSize();

		//Set each attrib
		for (int i = 0; i < l->size; i++)
		{
			glEnableVertexAttribArray(i);
			glVertexAttribPointer(i, l->elements[i].GetCount(), GetGLType(l->elements[i].format), false, stride, (void*)l->elements[i].offset);
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
		glDrawElements(GL_TRIANGLES, indexCount, GL_UNSIGNED_INT, 0);
	}

	ICBuffer* GLGraphics::CreateCBuffer(uint size, void* initData)
	{
		//Setup cbuffer
		GLCBuffer* cBuffer = new GLCBuffer();
		cBuffer->data = new char[size] { 0 };
		cBuffer->size = size;
		if (initData != nullptr)
			memcpy(cBuffer->data, initData, size);

		//Create and initialize CBuffer
		glGenBuffers(1, &cBuffer->buffer);
		glBindBuffer(GL_UNIFORM_BUFFER, cBuffer->buffer);
		glBufferData(GL_UNIFORM_BUFFER, size, cBuffer->data, GL_STATIC_DRAW);

		return cBuffer;
	}

	void* GLGraphics::GetCBufferPtr(ICBuffer* cBuffer)
	{
		return ((GLCBuffer*)cBuffer)->data;
	}

	uint GLGraphics::GetCBufferSize(ICBuffer* cBuffer)
	{
		return ((GLCBuffer*)cBuffer)->size;
	}

	void GLGraphics::UpdateCBuffer(ICBuffer* cBuffer)
	{
		glBindBuffer(GL_UNIFORM_BUFFER, ((GLCBuffer*)cBuffer)->buffer);
		glBufferSubData(GL_UNIFORM_BUFFER, 0, ((GLCBuffer*)cBuffer)->size, ((GLCBuffer*)cBuffer)->data);
	}

	void GLGraphics::BindCBuffer(ICBuffer* cBuffer, ShaderType type, uint slot)
	{
		glBindBufferBase(GL_UNIFORM_BUFFER, slot, ((GLCBuffer*)cBuffer)->buffer);
	}

	void GLGraphics::ReleaseCBuffer(ICBuffer** cBuffer)
	{
		//Free the data
		delete[]((GLCBuffer*)*cBuffer)->data;
		((GLCBuffer*)*cBuffer)->data = nullptr;

		glDeleteBuffers(1, &((GLCBuffer*)*cBuffer)->buffer);
		((GLCBuffer*)*cBuffer)->buffer = 0;

		delete *cBuffer;
		*cBuffer = nullptr;
	}

	ITexture* GLGraphics::CreateTexture(uint w, uint h, TextureFormat format, void* data, bool wrap, bool mipmaps, bool filter)
	{
		GLTexture* tex = new GLTexture();

		glGenTextures(1, &tex->texture);
		glBindTexture(GL_TEXTURE_2D, tex->texture);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, wrap ? GL_REPEAT : GL_CLAMP);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, wrap ? GL_REPEAT : GL_CLAMP);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, filter ? GL_LINEAR : GL_POINT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, filter ? GL_LINEAR : GL_POINT);

		uint glFormat = -1;
		switch (format)
		{
			case TextureFormat::R8:
				glFormat = GL_RED;
				break;
			case TextureFormat::R8G8:
				glFormat = GL_RG;
				break;
			case TextureFormat::R8G8B8A8:
				glFormat = GL_RGBA;
				break;
			default:
				glFormat = GL_RED;
		}

		//Set the data
		glTexImage2D(GL_TEXTURE_2D, 0, glFormat, w, h, 0, glFormat, GL_UNSIGNED_BYTE, data);
		if (mipmaps)
			glGenerateMipmap(GL_TEXTURE_2D);

		return tex;
	}

	void GLGraphics::BindTexture(ITexture* texture, uint slot)
	{
		GLTexture* tex = (GLTexture*)texture;

		glBindTexture(GL_TEXTURE_2D, tex->texture);

		glActiveTexture(GL_TEXTURE0 + slot);
		glBindTexture(GL_TEXTURE_2D, tex->texture);
	}

	void GLGraphics::ReleaseTexture(ITexture** texture)
	{
		if (!texture)
			return;

		delete *texture;
		*texture = nullptr;
	}

	void GLGraphics::ImGuiInit(const WinProp& windowProperties)
	{
	#ifdef LIL_ENABLE_IMGUI
		// Setup Platform/Renderer bindings
		ImGui_ImplWin32_Init(windowProperties.hwnd);
		ImGui_ImplOpenGL3_Init();
	#endif //LIL_ENABLE_IMGUI
	}

	void GLGraphics::ImGuiNewFrame()
	{
	#ifdef LIL_ENABLE_IMGUI
		// Start the Dear ImGui frame
		ImGui_ImplOpenGL3_NewFrame();
		ImGui_ImplWin32_NewFrame();
		ImGui::NewFrame();
	#endif //LIL_ENABLE_IMGUI
	}

	void GLGraphics::ImGuiRender()
	{
	#ifdef LIL_ENABLE_IMGUI
		ImGui::Render();
		ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
	#endif //LIL_ENABLE_IMGUI
	}

	void GLGraphics::ImGuiShutdown()
	{
	#ifdef LIL_ENABLE_IMGUI
		ImGui_ImplOpenGL3_Shutdown();
		ImGui_ImplWin32_Shutdown();
		ImGui::DestroyContext();
	#endif LIL_ENABLE_IMGUI
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
		for (int i = 0; i < numElements; i++)
			layout->elements[i] = elements[i];

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
			LIL_ERROR("OpenGL shader compilation error.", msg);
			delete[] msg;
		}

		return shader;
	}
}
