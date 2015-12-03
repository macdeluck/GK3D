#ifndef _GK_LIGHTINSTANCE
#define _GK_LIGHTINSTANCE

#include "Common.h"
#include "Drawable.h"

namespace GK
{
	class PointLightInstance : public DrawableInstance
	{
	public:
		GLfloat constantTerm;
		GLfloat linearTerm;
		GLfloat quadraticTerm;

		PointLightInstance(std::shared_ptr<ShaderProgram> shaderProgram,
			std::shared_ptr<Material> material,
			GLfloat constantTerm = 1.0f,
			GLfloat linearTerm = 0.09f,
			GLfloat quadraticTerm = 0.032f,
			glm::vec3 position = glm::vec3(0, 0, 0),
			glm::vec3 scale = glm::vec3(1, 1, 1),
			GLfloat angleX = 0, GLfloat angleY = 0, GLfloat angleZ = 0);
	};
}

#endif