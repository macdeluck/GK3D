#include "Fog.h"

namespace GK
{
	Fog::Fog()
		: density(0.0f), color(0.0f, 0.0f, 0.0f)
	{
	}

	Fog::Fog(float density, glm::vec3 color)
		: density(density), color(color)
	{
	}

	Fog::~Fog()
	{
	}
}
