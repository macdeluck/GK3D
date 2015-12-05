#ifndef _GK_DRAWABLE
#define _GK_DRAWABLE

#include "Common.h"
#include <list>

namespace GK
{
	class ShaderProgram;
	class Scene;
	class Image;

	struct Vertex;
	struct Material;
	class DrawableInstance;
	class Drawable;

	struct Vertex
	{
	public:
		static const int VERTEX_SIZE = 8;
		static const int VERTEX_POSITIONS = 3;
		static const int VERTEX_NORMALS = 3;
		static const int VERTEX_TEXCOORDS = 2;
		// 3f - position, 3f - normal, 2f - texcoords

		Vertex();
		Vertex(GLfloat vertexData[VERTEX_SIZE]);
		Vertex(std::initializer_list<GLfloat> initializer);

		std::vector<GLfloat> toVector();
		GLfloat& operator[](int i);
	private:
		GLfloat vertexData[VERTEX_SIZE];
	};

	struct Texture
	{
	private:
		std::shared_ptr<GLuint> texture;

	public:
		Texture();
		Texture(std::shared_ptr<Image> image);
		virtual ~Texture();
		bool empty();
		GLuint getId();

		friend class Drawable;
	};

	struct Material
	{
		glm::vec3 ambient;
		glm::vec3 diffuse;
		glm::vec3 specular;
		GLfloat shininess;
		Texture diffuseTex;
		Texture alphaTex;

		static const Material Identity;

		static const Material WhiteLight;
		static const Material RedLight;
		static const Material GreenLight;
		static const Material BlueLight;
		static const Material CyanLight;
		static const Material MagentaLight;
		static const Material YellowLight;

		static const Material Emerald;
		static const Material Jade;
		static const Material Obsidian;
		static const Material Pearl;
		static const Material Ruby;
		static const Material Turquoise;
		static const Material Brass;
		static const Material Bronze;
		static const Material Chrome;
		static const Material Copper;
		static const Material Gold;
		static const Material Silver;
		static const Material BlackPlastic;
		static const Material CyanPlastic;
		static const Material GreenPlastic;
		static const Material RedPlastic;
		static const Material WhitePlastic;
		static const Material YellowPlastic;
		static const Material BlackRubber;
		static const Material CyanRubber;
		static const Material GreenRubber;
		static const Material RedRubber;
		static const Material WhiteRubber;
		static const Material YellowRubber;

		Material(glm::vec3 ambient = glm::vec3(0, 0, 0),
			glm::vec3 diffuse = glm::vec3(0, 0, 0),
			glm::vec3 specular = glm::vec3(0, 0, 0),
			GLfloat shininess = 1.0f,
			Texture diffuseTex = Texture());
	};

	class DrawableInstance
	{
	public:
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

		virtual ~DrawableInstance();
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