#include "Common.h"
#include "Drawable.h"
#include "Scene.h"
#include "Shader.h"
#include "Image.h"
#include <algorithm>

namespace GK
{
	Material::Material(glm::vec3 ambient,
		glm::vec3 diffuse,
		glm::vec3 specular,
		GLfloat shininess,
		Texture diffuseTex)
		: ambient(ambient), diffuse(diffuse), specular(specular), shininess(shininess), diffuseTex(diffuseTex)
	{}
	
	DrawableInstance::DrawableInstance(std::shared_ptr<ShaderProgram> shaderProgram,
		std::shared_ptr<Material> material,
		glm::vec3 position,
		glm::vec3 scale,
		GLfloat angleX, GLfloat angleY, GLfloat angleZ)
		: shaderProgram(shaderProgram),
		material(material), position(position), scale(scale), angleX(angleX), angleY(angleY), angleZ(angleZ)
	{
	}

	DrawableInstance::~DrawableInstance()
	{
	}

	Drawable::Drawable(std::vector<Vertex> vertices, std::vector<GLuint> indices,
		std::vector<std::shared_ptr<DrawableInstance> > instances)
		: vertices(vertices), instances(instances), indices(indices), vao(new GLuint(0)), vbo(new GLuint(0)), ebo(new GLuint(0))
	{
		init();
	}

	Drawable::Drawable(std::vector<Vertex> vertices,
		std::vector<std::shared_ptr<DrawableInstance> > instances)
		: vertices(vertices), instances(instances), indices(0), vao(new GLuint(0)), vbo(new GLuint(0)), ebo(new GLuint(0))
	{
		init();
	}

	void Drawable::init()
	{
		{
			GLuint l_vao, l_vbo, l_ebo;
			GLRUN(glGenVertexArrays(1, &l_vao));
			*vao = l_vao;
			GLRUN(glGenBuffers(1, &l_vbo));
			*vbo = l_vbo;
			if (indices.size() > 0)
			{
				GLRUN(glGenBuffers(1, &l_ebo));
				*ebo = l_ebo;
			}
		}
		GLRUN(glBindVertexArray(*vao));

		GLRUN(glBindBuffer(GL_ARRAY_BUFFER, *vbo));
		std::vector<GLfloat> vertexData = getVertexData();
		GLRUN(glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat)*vertexData.size(), vertexData.data(), GL_STATIC_DRAW));

		GLRUN(glVertexAttribPointer(0, Vertex::VERTEX_POSITIONS, GL_FLOAT, GL_FALSE, Vertex::VERTEX_SIZE * sizeof(GLfloat), (GLvoid*)0));
		GLRUN(glEnableVertexAttribArray(0));
		GLRUN(glVertexAttribPointer(1, Vertex::VERTEX_NORMALS, GL_FLOAT, GL_FALSE, Vertex::VERTEX_SIZE * sizeof(GLfloat), (GLvoid*)(Vertex::VERTEX_POSITIONS * sizeof(GLfloat))));
		GLRUN(glEnableVertexAttribArray(1));
		GLRUN(glVertexAttribPointer(2, Vertex::VERTEX_TEXCOORDS, GL_FLOAT, GL_FALSE, Vertex::VERTEX_SIZE * sizeof(GLfloat), (GLvoid*)((Vertex::VERTEX_POSITIONS + Vertex::VERTEX_NORMALS) * sizeof(GLfloat))));
		GLRUN(glEnableVertexAttribArray(2));

		if (indices.size() > 0)
		{
			GLRUN(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, *ebo));
			GLRUN(glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(GLuint)*indices.size(), indices.data(), GL_STATIC_DRAW));
		}

		GLRUN(glBindBuffer(GL_ARRAY_BUFFER, 0));

		GLRUN(glBindVertexArray(0));
	}

	Drawable::~Drawable()
	{
		if (vao.unique())
		{
			glDeleteVertexArrays(1, &(*vao));
		}
		if (vbo.unique())
		{
			glDeleteBuffers(1, &(*vbo));
		}
		if (indices.size() > 0)
		{
			if (ebo.unique())
			{
				glDeleteBuffers(1, &(*ebo));
			}
		}
	}

	void Drawable::render(std::shared_ptr<Scene> scene)
	{
		GLRUN(glBindVertexArray(*vao));
		for (size_t i = 0; i < instances.size(); i++)
		{
			instances[i]->shaderProgram->preRender(instances[i], scene);
			instances[i]->shaderProgram->doRender(instances[i], scene);
			if (indices.size() > 0)
			{
				GLRUN(glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, 0));
			}
			else
			{
				GLRUN(glDrawArrays(GL_TRIANGLES, 0, vertices.size()));
			}
			instances[i]->shaderProgram->postRender(instances[i], scene);
		}
		GLRUN(glBindVertexArray(0));
	}

	std::vector<std::shared_ptr<DrawableInstance> >  Drawable::getInstances()
	{
		return instances;
	}

	std::vector<GLfloat> Drawable::getVertexData()
	{
		std::vector<GLfloat> result = std::vector<GLfloat>(vertices.size() * Vertex::VERTEX_SIZE);
		for (size_t i = 0; i < vertices.size(); i++)
		{
			std::copy_n(vertices[i].toVector().begin(), Vertex::VERTEX_SIZE, result.begin() + i*Vertex::VERTEX_SIZE);
		}
		return result;
	}

	Vertex::Vertex() 
	{
		for (size_t i = 0; i < VERTEX_SIZE; i++)
		{
			vertexData[i] = 0;
		}
	}
	Vertex::Vertex(GLfloat vertexData[VERTEX_SIZE])
	{
		for (size_t i = 0; i < VERTEX_SIZE; i++)
		{
			this->vertexData[i] = vertexData[i];
		}
	}
	Vertex::Vertex(std::initializer_list<GLfloat> initializer)
	{
		assert(initializer.size() == VERTEX_SIZE);
		size_t i = 0;
		for (std::initializer_list<GLfloat>::const_iterator it = initializer.begin();
			it != initializer.end(); ++it)
		{
			this->vertexData[i++] = *it;
		}
	}

	std::vector<GLfloat> Vertex::toVector()
	{
		return std::vector<GLfloat>(std::begin(vertexData), std::end(vertexData));
	}

	Texture::Texture()
		: texture(nullptr)
	{
	}

	Texture::Texture(std::shared_ptr<Image> image)
		: texture(new GLuint)
	{
		GLRUN(glGenTextures(1, &(*texture)));
		GLRUN(glBindTexture(GL_TEXTURE_2D, (*texture)));
		if (image->getImageChannels() == ImageChannels::ImageRGB)
		{
			GLRUN(glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, image->getWidth(), image->getHeight(), 0, GL_RGB, GL_UNSIGNED_BYTE, image->getData().data()));
		}
		else
		{
			GLRUN(glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, image->getWidth(), image->getHeight(), 0, GL_RGBA, GL_UNSIGNED_BYTE, image->getData().data()));
		}
		GLRUN(glGenerateMipmap(GL_TEXTURE_2D));
		GLRUN(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT));
		GLRUN(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT));
		GLRUN(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR));
		GLRUN(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR));
		GLRUN(glBindTexture(GL_TEXTURE_2D, 0));
	}

	Texture::~Texture()
	{
		if (texture.unique())
			glDeleteTextures(1, &(*texture));
	}

	bool Texture::empty()
	{
		return !(bool)texture;
	}

	GLuint Texture::getId()
	{
		return *texture;
	}

	const Material Material::Identity = Material(glm::vec3(1.0f, 1.0f, 1.0f), glm::vec3(1.0f, 1.0f, 1.0f), glm::vec3(1.0f, 1.0f, 1.0f), 1.0f);

	const Material Material::WhiteLight = Material(glm::vec3(0.2f, 0.2f, 0.2f), glm::vec3(0.5f, 0.5f, 0.5f), glm::vec3(1.0f, 1.0f, 1.0f), 1.0f);
	const Material Material::RedLight = Material(glm::vec3(0.2f, 0.0f, 0.0f), glm::vec3(0.5f, 0.0f, 0.0f), glm::vec3(1.0f, 0.0f, 0.0f), 1.0f);
	const Material Material::GreenLight = Material(glm::vec3(0.0f, 0.2f, 0.0f), glm::vec3(0.0f, 0.5f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f), 1.0f);
	const Material Material::BlueLight = Material(glm::vec3(0.0f, 0.0f, 0.2f), glm::vec3(0.0f, 0.0f, 0.5f), glm::vec3(0.0f, 0.0f, 1.0f), 1.0f);
	const Material Material::CyanLight = Material(glm::vec3(0.0f, 0.2f, 0.2f), glm::vec3(0.0f, 0.5f, 0.5f), glm::vec3(0.0f, 1.0f, 1.0f), 1.0f);
	const Material Material::MagentaLight = Material(glm::vec3(0.2f, 0.0f, 0.2f), glm::vec3(0.5f, 0.0f, 0.5f), glm::vec3(1.0f, 0.0f, 1.0f), 1.0f);
	const Material Material::YellowLight = Material(glm::vec3(0.2f, 0.2f, 0.0f), glm::vec3(0.5f, 0.5f, 0.0f), glm::vec3(1.0f, 1.0f, 0.0f), 1.0f);

	const Material Material::Emerald = Material(glm::vec3(0.0215f, 0.1745f, 0.0215f), glm::vec3(0.07568f, 0.61424f, 0.07568f), glm::vec3(0.633f, 0.727811f, 0.633f), 76.8f);
	const Material Material::Jade = Material(glm::vec3(0.135f, 0.2225f, 0.1575f), glm::vec3(0.54f, 0.89f, 0.63f), glm::vec3(0.316228f, 0.316228f, 0.316228f), 12.8f);
	const Material Material::Obsidian = Material(glm::vec3(0.05375f, 0.05f, 0.06625f), glm::vec3(0.18275f, 0.17f, 0.22525f), glm::vec3(0.332741f, 0.328634f, 0.346435f), 38.4f);
	const Material Material::Pearl = Material(glm::vec3(0.25f, 0.20725f, 0.20725f), glm::vec3(1.0f, 0.829f, 0.829f), glm::vec3(0.296648f, 0.296648f, 0.296648f), 11.264f);
	const Material Material::Ruby = Material(glm::vec3(0.1745f, 0.01175f, 0.01175f), glm::vec3(0.61424f, 0.04136f, 0.04136f), glm::vec3(0.727811f, 0.626959f, 0.626959f), 76.8f);
	const Material Material::Turquoise = Material(glm::vec3(0.1f, 0.18725f, 0.1745f), glm::vec3(0.396f, 0.74151f, 0.69102f), glm::vec3(0.297254f, 0.30829f, 0.306678f), 12.8f);
	const Material Material::Brass = Material(glm::vec3(0.329412f, 0.223529f, 0.027451f), glm::vec3(0.780392f, 0.568627f, 0.113725f), glm::vec3(0.992157f, 0.941176f, 0.807843f), 27.89743616f);
	const Material Material::Bronze = Material(glm::vec3(0.2125f, 0.1275f, 0.054f), glm::vec3(0.714f, 0.4284f, 0.18144f), glm::vec3(0.393548f, 0.271906f, 0.166721f), 25.6f);
	const Material Material::Chrome = Material(glm::vec3(0.25f, 0.25f, 0.25f), glm::vec3(0.4f, 0.4f, 0.4f), glm::vec3(0.774597f, 0.774597f, 0.774597f), 76.8f);
	const Material Material::Copper = Material(glm::vec3(0.19125f, 0.0735f, 0.0225f), glm::vec3(0.7038f, 0.27048f, 0.0828f), glm::vec3(0.256777f, 0.137622f, 0.086014f), 12.8f);
	const Material Material::Gold = Material(glm::vec3(0.24725f, 0.1995f, 0.0745f), glm::vec3(0.75164f, 0.60648f, 0.22648f), glm::vec3(0.628281f, 0.555802f, 0.366065f), 51.2f);
	const Material Material::Silver = Material(glm::vec3(0.19225f, 0.19225f, 0.19225f), glm::vec3(0.50754f, 0.50754f, 0.50754f), glm::vec3(0.508273f, 0.508273f, 0.508273f), 51.2f);
	const Material Material::BlackPlastic = Material(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.01f, 0.01f, 0.01f), glm::vec3(0.5f, 0.5f, 0.5f), 32.0f);
	const Material Material::CyanPlastic = Material(glm::vec3(0.0f, 0.1f, 0.06f), glm::vec3(0.0f, 0.50980392f, 0.50980392f), glm::vec3(0.50196078f, 0.50196078f, 0.50196078f), 32.0f);
	const Material Material::GreenPlastic = Material(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.1f, 0.35f, 0.1f), glm::vec3(0.45f, 0.55f, 0.45f), 32.0f);
	const Material Material::RedPlastic = Material(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.5f, 0.0f, 0.0f), glm::vec3(0.7f, 0.6f, 0.6f), 32.0f);
	const Material Material::WhitePlastic = Material(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.55f, 0.55f, 0.55f), glm::vec3(0.7f, 0.7f, 0.7f), 32.0f);
	const Material Material::YellowPlastic = Material(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.5f, 0.5f, 0.0f), glm::vec3(0.6f, 0.6f, 0.5f), 32.0f);
	const Material Material::BlackRubber = Material(glm::vec3(0.02f, 0.02f, 0.02f), glm::vec3(0.01f, 0.01f, 0.01f), glm::vec3(0.4f, 0.4f, 0.4f), 10.0f);
	const Material Material::CyanRubber = Material(glm::vec3(0.0f, 0.05f, 0.05f), glm::vec3(0.4f, 0.5f, 0.5f), glm::vec3(0.04f, 0.7f, 0.7f), 10.0f);
	const Material Material::GreenRubber = Material(glm::vec3(0.0f, 0.05f, 0.0f), glm::vec3(0.4f, 0.5f, 0.4f), glm::vec3(0.04f, 0.7f, 0.04f), 10.0f);
	const Material Material::RedRubber = Material(glm::vec3(0.05f, 0.0f, 0.0f), glm::vec3(0.5f, 0.4f, 0.4f), glm::vec3(0.7f, 0.04f, 0.04f), 10.0f);
	const Material Material::WhiteRubber = Material(glm::vec3(0.05f, 0.05f, 0.05f), glm::vec3(0.5f, 0.5f, 0.5f), glm::vec3(0.7f, 0.7f, 0.7f), 10.0f);
	const Material Material::YellowRubber = Material(glm::vec3(0.05f, 0.05f, 0.0f), glm::vec3(0.5f, 0.5f, 0.4f), glm::vec3(0.7f, 0.7f, 0.04f), 10.0f);
}