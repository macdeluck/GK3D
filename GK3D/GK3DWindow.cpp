#include "GK3DWindow.h"

namespace GK
{
	GK3DWindow::GK3DWindow(int width, int height, std::string title, bool shown, bool resizable)
		: Window(width, height, title, shown, resizable) 
	{
		std::shared_ptr<Shader> vertexShader = std::shared_ptr<Shader>(new Shader(Shader::FromFile("vertex_shader.glsl", ShaderType::Vertex)));
		std::shared_ptr<Shader> fragmentShader = std::shared_ptr<Shader>(new Shader(Shader::FromFile("fragment_shader.glsl", ShaderType::Fragment)));
		std::shared_ptr<ShaderProgram> shaderProgram(new ShaderProgram(vertexShader, fragmentShader));
	
		std::vector<GLfloat> vertices = {
			0.5f, 0.5f, 0.0f,  // Top Right
			0.5f, -0.5f, 0.0f,  // Bottom Right
			-0.5f, -0.5f, 0.0f,  // Bottom Left
			-0.5f, 0.5f, 0.0f   // Top Left 
		};
		std::vector<GLuint> indices = {
			0, 1, 3,  // First Triangle
			1, 2, 3   // Second Triangle
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

	}
}