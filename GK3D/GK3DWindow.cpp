#include "GK3DWindow.h"
#include <cmath>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

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
		int screenWidth, screenHeight;

		GK3DShaderProgram()
			: ShaderProgram(std::shared_ptr<GK3DVertexShader>(new GK3DVertexShader()),
			std::shared_ptr<GK3DFragmentShader>(new GK3DFragmentShader()))
		{
		}
		virtual ~GK3DShaderProgram() {}
		virtual void update() override
		{
			glm::mat4 model;
			glm::mat4 view;
			glm::mat4 projection;
			model = glm::rotate(model, glm::radians(-55.0f), glm::vec3(1.0f, 0.0f, 0.0f));
			view = glm::translate(view, glm::vec3(0.0f, 0.0f, -3.0f));
			projection = glm::perspective(45.0f, ((float)screenWidth) / screenHeight, 0.1f, 100.0f);
			glUniformMatrix4fv(getUniformLocation("model"), 1, GL_FALSE, glm::value_ptr(model));
			glUniformMatrix4fv(getUniformLocation("view"), 1, GL_FALSE, glm::value_ptr(view));
			glUniformMatrix4fv(getUniformLocation("projection"), 1, GL_FALSE, glm::value_ptr(projection));
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
		shaderProgram->screenWidth = width;
		shaderProgram->screenHeight = height;
		std::vector<Vertex> vertices = {
			// Positions         // Colors
			Vertex(-0.5f, -0.5f, -0.5f, 0.0f, 0.0f, 0.0f),
			Vertex(0.5f, -0.5f, -0.5f, 1.0f, 0.0f, 0.0f),
			Vertex(0.5f, 0.5f, -0.5f, 1.0f, 1.0f, 0.0f),
			Vertex(0.5f, 0.5f, -0.5f, 1.0f, 1.0f, 0.0f),
			Vertex(-0.5f, 0.5f, -0.5f, 0.0f, 1.0f, 0.0f),
			Vertex(-0.5f, -0.5f, -0.5f, 0.0f, 0.0f, 0.0f),

			Vertex(-0.5f, -0.5f, 0.5f, 0.0f, 0.0f, 0.0f),
			Vertex(0.5f, -0.5f, 0.5f, 1.0f, 0.0f, 0.0f),
			Vertex(0.5f, 0.5f, 0.5f, 1.0f, 1.0f, 0.0f),
			Vertex(0.5f, 0.5f, 0.5f, 1.0f, 1.0f, 0.0f),
			Vertex(-0.5f, 0.5f, 0.5f, 0.0f, 1.0f, 0.0f),
			Vertex(-0.5f, -0.5f, 0.5f, 0.0f, 0.0f, 0.0f),

			Vertex(-0.5f, 0.5f, 0.5f, 1.0f, 0.0f, 0.0f),
			Vertex(-0.5f, 0.5f, -0.5f, 1.0f, 1.0f, 0.0f),
			Vertex(-0.5f, -0.5f, -0.5f, 0.0f, 1.0f, 0.0f),
			Vertex(-0.5f, -0.5f, -0.5f, 0.0f, 1.0f, 0.0f),
			Vertex(-0.5f, -0.5f, 0.5f, 0.0f, 0.0f, 0.0f),
			Vertex(-0.5f, 0.5f, 0.5f, 1.0f, 0.0f, 0.0f),

			Vertex(0.5f, 0.5f, 0.5f, 1.0f, 0.0f, 0.0f),
			Vertex(0.5f, 0.5f, -0.5f, 1.0f, 1.0f, 0.0f),
			Vertex(0.5f, -0.5f, -0.5f, 0.0f, 1.0f, 0.0f),
			Vertex(0.5f, -0.5f, -0.5f, 0.0f, 1.0f, 0.0f),
			Vertex(0.5f, -0.5f, 0.5f, 0.0f, 0.0f, 0.0f),
			Vertex(0.5f, 0.5f, 0.5f, 1.0f, 0.0f, 0.0f),

			Vertex(-0.5f, -0.5f, -0.5f, 0.0f, 1.0f, 0.0f),
			Vertex(0.5f, -0.5f, -0.5f, 1.0f, 1.0f, 0.0f),
			Vertex(0.5f, -0.5f, 0.5f, 1.0f, 0.0f, 0.0f),
			Vertex(0.5f, -0.5f, 0.5f, 1.0f, 0.0f, 0.0f),
			Vertex(-0.5f, -0.5f, 0.5f, 0.0f, 0.0f, 0.0f),
			Vertex(-0.5f, -0.5f, -0.5f, 0.0f, 1.0f, 0.0f),

			Vertex(-0.5f, 0.5f, -0.5f, 0.0f, 1.0f, 0.0f),
			Vertex(0.5f, 0.5f, -0.5f, 1.0f, 1.0f, 0.0f),
			Vertex(0.5f, 0.5f, 0.5f, 1.0f, 0.0f, 0.0f),
			Vertex(0.5f, 0.5f, 0.5f, 1.0f, 0.0f, 0.0f),
			Vertex(-0.5f, 0.5f, 0.5f, 0.0f, 0.0f, 0.0f),
			Vertex(-0.5f, 0.5f, -0.5f, 0.0f, 1.0f, 0.0f)
		};
		box.reset(new Drawable(vertices, shaderProgram));
	}
	GK3DWindow::GK3DWindow(const GK3DWindow& otherWindow) : Window(otherWindow) {}
	GK3DWindow& GK3DWindow::operator=(const GK3DWindow& otherWindow)
	{
		Window::operator=(otherWindow);
		return *this;
	}
	GK3DWindow::~GK3DWindow() {}

	void GK3DWindow::handleEvent(SDL_Event& event)
	{
		Window::handleEvent(event);
		std::shared_ptr<GK3DShaderProgram> shader = std::dynamic_pointer_cast<GK3DShaderProgram>(box->getShader());
		shader->screenWidth = getWidth();
		shader->screenHeight = getHeight();
	}

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