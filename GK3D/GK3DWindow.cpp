#include "GK3DWindow.h"
#include <cmath>

namespace GK
{
	class GK3DVertexShader : public Shader
	{
	public:
		GK3DVertexShader() : Shader(Shader::FromFile("vertex_shader.glsl"), ShaderType::VertexShader) {}
		virtual ~GK3DVertexShader() {}
	};
	class GK3DFragmentShader : public Shader
	{
	public:
		GK3DFragmentShader() : Shader(Shader::FromFile("fragment_shader.glsl"), ShaderType::FragmentShader) {}
		virtual ~GK3DFragmentShader() {}
	};

	class GK3DShaderProgram : public ShaderProgram
	{
	public:
		GK3DShaderProgram()
			: ShaderProgram(std::shared_ptr<GK3DVertexShader>(new GK3DVertexShader()),
			std::shared_ptr<GK3DFragmentShader>(new GK3DFragmentShader())) 
		{
		}
		virtual ~GK3DShaderProgram() {}
		virtual void update() override
		{
			/*GLfloat timeValue = ((float)SDL_GetTicks())/1000;
			GLfloat greenValue = (sin(timeValue) / 2) + 0.5;
			GLuint shaderId = getProgramId();
			GLint vertexColorLocation = glGetUniformLocation(shaderId, "timeColor");
			if (vertexColorLocation == 0xffffffff)
				throw Exception("Uniform location was not found");
			glUniform4f(vertexColorLocation, 0.0f, greenValue, 0.0f, 1.0f);*/
		}
		virtual void before_link() override
		{
			glBindAttribLocation(getProgramId(), 0, "position");
			glBindAttribLocation(getProgramId(), 1, "color");
		}
	};

	GK3DWindow::GK3DWindow(int width, int height, std::string title, bool shown, bool resizable)
		: Window(width, height, title, shown, resizable) 
	{
		std::shared_ptr<GK3DShaderProgram> shaderProgram(new GK3DShaderProgram());
		shaderProgram->compile();
		std::vector<Vertex> vertices = {
			// Positions         // Colors
			Vertex(0.5f, -0.5f, 0.0f, 1.0f, 0.0f, 0.0f),  // Bottom Right R
			Vertex(-0.5f, -0.5f, 0.0f, 0.0f, 1.0f, 0.0f),  // Bottom Left G
			Vertex(0.0f, 0.5f, 0.0f, 0.0f, 0.0f, 1.0f) // Top B
		};
		std::vector<GLuint> indices = {
			0, 1, 2
		};
		box.reset(new Drawable(vertices, indices, shaderProgram));
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