#include "LightSourceInstance.h"
#include "LightShader.h"

namespace GK
{
	LightSourceInstance::LightSourceInstance(std::shared_ptr<ShaderProgram> shaderProgram,
		std::shared_ptr<Material> material,
		GLfloat constantTerm,
		GLfloat linearTerm,
		GLfloat quadraticTerm,
		glm::vec3 position,
		glm::vec3 scale,
		GLfloat angleX, GLfloat angleY, GLfloat angleZ)
		: DrawableInstance(shaderProgram, material, position, scale, angleX, angleY, angleZ),
		constantTerm(constantTerm), linearTerm(linearTerm), quadraticTerm(quadraticTerm)
	{
	}
}