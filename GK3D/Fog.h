#ifndef _GK_FOG
#define _GK_FOG

#include "Common.h"

namespace GK
{
	struct Fog
	{
		float density;
		glm::vec3 color;

		Fog();
		Fog(float density, glm::vec3 color);
		virtual ~Fog();
	};
}
#endif