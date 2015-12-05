#ifndef _GK_RANDOMNOISE
#define _GK_RANDOMNOISE

#include "INoise.h"
#include <random>

namespace GK
{
	class RandomNoise : public INoise
	{
	private:
		unsigned int seed;
		float minVal;
		float maxVal;
		std::default_random_engine generator;
		std::uniform_real_distribution<float> distribution;
	public:
		RandomNoise(unsigned int seed, float minVal, float maxVal);
		virtual ~RandomNoise();
		virtual std::vector<float> generate(size_t n);
		virtual unsigned int getSeed() const;
		virtual unsigned int getMinVal() const;
		virtual unsigned int getMaxVal() const;
	};
}

#endif