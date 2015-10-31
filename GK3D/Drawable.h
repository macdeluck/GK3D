#ifndef _GK_DRAWABLE
#define _GK_DRAWABLE

#include "Common.h"

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

	struct Material
	{
		glm::vec3 ambient;
		glm::vec3 diffuse;
		glm::vec3 specular;
		GLfloat shininess;

		Material(glm::vec3 ambient = glm::vec3(0, 0, 0),
			glm::vec3 diffuse = glm::vec3(0, 0, 0),
			glm::vec3 specular = glm::vec3(0, 0, 0),
			GLfloat shininess = 0.0f);
	};

	struct DrawableInstance
	{
		std::shared_ptr<Material> material;
		glm::vec3 position;
		glm::vec3 scale;
		GLfloat angleX;
		GLfloat angleY;
		GLfloat angleZ;
		std::shared_ptr<ShaderProgram> shaderProgram;

		DrawableInstance(std::shared_ptr<ShaderProgram> shaderProgram,
			std::shared_ptr<Material> material,
			glm::vec3 position = glm::vec3(0, 0, 0),
			glm::vec3 scale = glm::vec3(1, 1, 1),
			GLfloat angleX = 0, GLfloat angleY = 0, GLfloat angleZ = 0);
	};

	class Drawable
	{
	public:
		Drawable(std::vector<Vertex> vertices,
			std::vector<std::shared_ptr<DrawableInstance> > instances);
		Drawable(std::vector<Vertex> vertices,
			std::vector<GLuint> indices,
			std::vector<std::shared_ptr<DrawableInstance> > instances);
		virtual ~Drawable();
		std::vector<std::shared_ptr<DrawableInstance> > getInstances();
		void render(std::shared_ptr<Scene> scene);
	private:
		void init();

		std::shared_ptr<GLuint> vbo;
		std::shared_ptr<GLuint> ebo;
		std::shared_ptr<GLuint> vao;
		std::vector<std::shared_ptr<DrawableInstance> > instances;
		std::vector<Vertex> vertices;
		std::vector<GLuint> indices;
		std::vector<GLfloat> getVertexData();
	};
}

#endif