#include "RandomNoise.h"

namespace GK
{

	RandomNoise::RandomNoise(unsigned int seed, float minVal, float maxVal) : INoise(), seed(seed), minVal(minVal), maxVal(maxVal)
	{
		generator = std::default_random_engine(seed);
		distribution = std::uniform_real_distribution<float>(minVal, maxVal);
	}
	RandomNoise::~RandomNoise()
	{
	}
	std::vector<float> RandomNoise::generate(size_t n)
	{
		auto result = std::vector<float>(n);
		for (size_t i = 0; i < result.size(); i++)
			result[i] = distribution(generator);
		return result;
	}
	unsigned int RandomNoise::getSeed() const
	{
		return seed;
	}
	unsigned int RandomNoise::getMinVal() const
	{
		return minVal;
	}
	unsigned int RandomNoise::getMaxVal() const
	{
		return maxVal;
	}
}
