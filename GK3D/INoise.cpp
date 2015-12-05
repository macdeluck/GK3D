#include "INoise.h"

namespace GK
{
	INoise::INoise()
	{
	}
	INoise::~INoise()
	{
	}
	std::vector<float> INoise::operator()(size_t n)
	{
		return generate(n);
	}
}