#include "GK3DWindow.h"
#include <cmath>

namespace GK
{
	class GK3DVertexShader : public Shader
	{
	public:
		GK3DVertexShader() : Shader(Shader::FromFile("vertex_shader.glsl"), ShaderType::Vertex) {}
		virtual ~GK3DVertexShader() {}
	};
	class GK3DFragmentShader : public Shader
	{
	public:
		GK3DFragmentShader() : Shader(Shader::FromFile("fragment_shader.glsl"), ShaderType::Fragment) {}
		virtual ~GK3DFragmentShader() {}
	};

	class GK3DShaderProgram : public ShaderProgram
	{
	public:
		GK3DShaderProgram()
			: ShaderProgram(std::shared_ptr<GK3DVertexShader>(new GK3DVertexShader()),
			std::shared_ptr<GK3DFragmentShader>(new GK3DFragmentShader())) {}
		virtual ~GK3DShaderProgram() {}
		virtual void update() override
		{
			GLfloat timeValue = ((float)SDL_GetTicks())/1000;
			GLfloat greenValue = (sin(timeValue) / 2) + 0.5;
			GLuint shaderId = getProgramId();
			GLint vertexColorLocation = glGetUniformLocation(shaderId, "timeColor");
			if (vertexColorLocation == 0xffffffff)
				throw Exception("Uniform location was not found");
			glUniform4f(vertexColorLocation, 0.0f, greenValue, 0.0f, 1.0f);
		}
	};

	GK3DWindow::GK3DWindow(int width, int height, std::string title, bool shown, bool resizable)
		: Window(width, height, title, shown, resizable) 
	{
		std::shared_ptr<GK3DShaderProgram> shaderProgram(new GK3DShaderProgram());
	
		std::vector<GLfloat> vertices = {
			0.5f, -0.5f, 0.0f,  // Bottom Right
			-0.5f, -0.5f, 0.0f,  // Bottom Left
			0.0f, 0.5f, 0.0f   // Top 
		};
		std::vector<GLuint> indices = {
			0, 1, 2
		};
		box.reset(new Drawable(vertices, indices, 3, shaderProgram));
	}
	GK3DWindow::GK3DWindow(const GK3DWindow& otherWindow) : Window(otherWindow) {}
	GK3DWindow& GK3DWindow::operator=(const GK3DWindow& otherWindow)
	{
		Window::operator=(otherWindow);
		return *this;
	}
	GK3DWindow::~GK3DWindow() {}


	void GK3DWindow::on_render()
	{
		box->render();
	}
	void GK3DWindow::on_update()
	{
		box->getShader()->use();
		box->getShader()->update();
	}
}