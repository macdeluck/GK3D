#ifndef _GK_SPOTLIGHTINSTANCE
#define _GK_SPOTLIGHTINSTANCE

#include "PointLightInstance.h"

namespace GK
{
	class SpotLightInstance : public PointLightInstance
	{
	public:
		glm::vec3 direction;
		GLfloat cutOff;
		GLfloat outerCutOff;

		SpotLightInstance(std::shared_ptr<ShaderProgram> shaderProgram,
			std::shared_ptr<Material> material,
			glm::vec3 direction = glm::vec3(0.0f, 0.0f, 1.0f),
			GLfloat cutOff = 12.5f,
			GLfloat outerCutOff = 17.5f,
			GLfloat constantTerm = 1.0f,
			GLfloat linearTerm = 0.09f,
			GLfloat quadraticTerm = 0.032f,
			glm::vec3 position = glm::vec3(0, 0, 0),
			glm::vec3 scale = glm::vec3(1, 1, 1),
			GLfloat angleX = 0, GLfloat angleY = 0, GLfloat angleZ = 0);
	};
}

#endif