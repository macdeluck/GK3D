#include "SpotLightInstance.h"

namespace GK
{
	SpotLightInstance::SpotLightInstance(std::shared_ptr<ShaderProgram> shaderProgram,
		std::shared_ptr<Material> material,
		glm::vec3 direction,
		GLfloat cutOff,
		GLfloat outerCutOff,
		GLfloat constantTerm,
		GLfloat linearTerm,
		GLfloat quadraticTerm,
		glm::vec3 position,
		glm::vec3 scale,
		GLfloat angleX, GLfloat angleY, GLfloat angleZ)
		: LightSourceInstance(shaderProgram, material,
		constantTerm, linearTerm, quadraticTerm, position, scale, angleX, angleY, angleZ),
		direction(direction), cutOff(cutOff), outerCutOff(outerCutOff)
	{
	}
}