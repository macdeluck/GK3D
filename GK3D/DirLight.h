#ifndef _GK_DIRLIGHT
#define _GK_DIRLIGHT

#include "Common.h"

namespace GK
{
	struct DirLight {
		glm::vec3 direction;

		glm::vec3 ambient;
		glm::vec3 diffuse;
		glm::vec3 specular;

		DirLight(glm::vec3 direction = glm::vec3(0.0f, 0.0f, 0.0f),
			glm::vec3 ambient = glm::vec3(0.0f, 0.0f, 0.0f),
			glm::vec3 diffuse = glm::vec3(0.0f, 0.0f, 0.0f),
			glm::vec3 specular = glm::vec3(0.0f, 0.0f, 0.0f))
			: direction(direction), ambient(ambient), diffuse(diffuse), specular(specular) {}
	};
}

#endif