#ifndef _GK_PERLINNOISE2D
#define _GK_PERLINNOISE2D

#include "PerlinNoise.h"

namespace GK
{
	class PerlinNoise2D : public PerlinNoise
	{
	private:
		float minVal;
		float maxVal;
	public:
		PerlinNoise2D(unsigned int seed, float minVal, float maxVal, unsigned int depth);
		virtual ~PerlinNoise2D();
		virtual std::vector<float> generate(size_t n);
		virtual std::vector<std::vector<float> > generate(size_t xn, size_t yn);
	};
}

#endif