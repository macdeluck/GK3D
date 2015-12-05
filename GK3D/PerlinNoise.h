#ifndef _GK_PERLINNOISE
#define _GK_PERLINNOISE

#include "INoise.h"
#include "RandomNoise.h"

namespace GK
{
	class PerlinNoise : public INoise
	{
	private:
		RandomNoise randomNoise;
		unsigned int depth;
		float minVal;
		float maxVal;
	public:
		PerlinNoise(unsigned int seed, float minVal, float maxVal, unsigned int depth);
		virtual ~PerlinNoise();
		virtual std::vector<float> generate(size_t n);
		virtual unsigned int getSeed() const;
		virtual float getMinVal() const;
		virtual float getMaxVal() const;
	};
}

#endif