#include "GK3DWindow.h"

namespace GK
{
	class GK3DVertexShader : public Shader
	{
	public:
		GK3DVertexShader() : Shader(Shader::FromFile("vertex_shader.glsl"), ShaderType::Vertex) {}
		virtual ~GK3DVertexShader() {}
		virtual void update() override
		{

		}
	};
	class GK3DFragmentShader : public Shader
	{
	public:
		GK3DFragmentShader() : Shader(Shader::FromFile("fragment_shader.glsl"), ShaderType::Fragment) {}
		virtual ~GK3DFragmentShader() {}
	};

	GK3DWindow::GK3DWindow(int width, int height, std::string title, bool shown, bool resizable)
		: Window(width, height, title, shown, resizable) 
	{
		std::shared_ptr<GK3DVertexShader> vertexShader = std::shared_ptr<GK3DVertexShader>(new GK3DVertexShader());
		std::shared_ptr<GK3DFragmentShader> fragmentShader = std::shared_ptr<GK3DFragmentShader>(new GK3DFragmentShader());
		std::shared_ptr<ShaderProgram> shaderProgram(new ShaderProgram(vertexShader, fragmentShader));
	
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
		box->getShader()->getVertexShader()->update();
		box->getShader()->getFragmentShader()->update();
	}
}