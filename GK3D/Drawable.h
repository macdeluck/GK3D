#ifndef _GK_DRAWABLE
#define _GK_DRAWABLE

#include "Common.h"
#include <array>

namespace GK
{
	class ShaderProgram;
	class Scene;

	struct Vertex
	{
	public:
		static const int VERTEX_SIZE = 6;

		Vertex();
		Vertex(GLfloat vertexData[VERTEX_SIZE]);
		Vertex(std::initializer_list<GLfloat> initializer);

		std::vector<GLfloat> toVector();
	private:
		GLfloat vertexData[VERTEX_SIZE];
	};

	struct DrawableInstance
	{
		glm::vec3 color;
		glm::vec3 position;
		glm::vec3 scale;
		GLfloat angleX;
		GLfloat angleY;
		GLfloat angleZ;
		std::shared_ptr<ShaderProgram> shaderProgram;

		DrawableInstance(std::shared_ptr<ShaderProgram> shaderProgram,
			glm::vec3 color = glm::vec3(0, 0, 0),
			glm::vec3 position = glm::vec3(0, 0, 0),
			glm::vec3 scale = glm::vec3(1, 1, 1),
			GLfloat angleX = 0, GLfloat angleY = 0, GLfloat angleZ = 0);
	};

	class Drawable
	{
	public:
		Drawable(std::vector<Vertex> vertices,
			std::vector<DrawableInstance> instances);
		Drawable(std::vector<Vertex> vertices,
			std::vector<GLuint> indices,
			std::vector<DrawableInstance> instances);
		virtual ~Drawable();
		std::vector<DrawableInstance> getInstances();
		void render(std::weak_ptr<Scene> scene);
	private:
		void init();

		std::shared_ptr<GLuint> vbo;
		std::shared_ptr<GLuint> ebo;
		std::shared_ptr<GLuint> vao;
		std::vector<DrawableInstance> instances;
		std::vector<Vertex> vertices;
		std::vector<GLuint> indices;
		std::vector<GLfloat> getVertexData();
	};
}

#endif