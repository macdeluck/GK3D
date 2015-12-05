#include "PerlinNoise.h"

namespace GK
{
	PerlinNoise::PerlinNoise(unsigned int seed, float minVal, float maxVal, unsigned int depth)
		: randomNoise(seed,  minVal / ((float)(1<<depth)) , maxVal / ((float)(1 << depth))), depth(depth), minVal(minVal), maxVal(maxVal)
	{
	}
	PerlinNoise::~PerlinNoise()
	{
	}

	namespace _perlin_noise
	{
		const float eps = 1e-5f;

		float abs(float x)
		{
			return x < 0 ? -x : x;
		}

		bool equals(float x, float y)
		{
			return abs(x - y) < eps;
		}

		int floor(float x)
		{
			if (x >= 0)
			{
				return (int)x;
			}
			else
			{
				int y = (int)x;
				return equals((float)y, x) ? y : y - 1;
			}
		}

		int ceil(float x)
		{
			if (x >= 0)
			{
				int y = (int)x;
				return equals((float)y, x) ? y : y + 1;
			}
			else
			{
				return (int)x;
			}
		}
	}

	std::vector<float> PerlinNoise::generate(size_t n)
	{
		std::vector<float> generation = randomNoise.generate(n);
		std::vector<float> result = std::vector<float>(generation.begin(), generation.end());
		for (size_t i = 0; i < depth; i++)
		{
			std::vector<float> nextGen = std::vector<float>(n);
			for (size_t j = 0; j < n; j++)
			{
				nextGen[j] = generation[_perlin_noise::floor(j / 2.0f)] + generation[_perlin_noise::ceil(j / 2.0f)];
				result[j] += nextGen[j];
			}
			generation = nextGen;
		}
		return result;
	}
	unsigned int PerlinNoise::getSeed() const
	{
		return randomNoise.getSeed();
	}
	float PerlinNoise::getMinVal() const
	{
		return minVal;
	}
	float PerlinNoise::getMaxVal() const
	{
		return maxVal;
	}
}
