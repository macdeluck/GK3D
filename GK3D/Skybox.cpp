#include "Skybox.h"

namespace GK
{
	SkyBox::SkyBox(std::shared_ptr<ShaderProgram> shaderProgram, std::shared_ptr<Texture3D> skyboxTexture,
		glm::vec3 scale, GLfloat angleX, GLfloat angleY, GLfloat angleZ)
		: DrawableInstance(shaderProgram, std::shared_ptr<Material>(new Material(Material::Identity)), glm::vec3(0.0f, 0.0f, 0.0f), scale, angleX, angleY, angleZ),
		skyboxTexture(skyboxTexture)
	{
	}

	SkyBox::~SkyBox()
	{
	}
}