#include "PerlinNoise2D.h"
#include <cassert>

namespace GK
{
	namespace _perlin_noise2d
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

		float sqrt(float val)
		{
			float i = 0;
			float x1, x2;
			while ((i*i) <= val)
				i += 0.1f;
			x1 = i;
			for (int j = 0;j<10;j++)
			{
				x2 = val;
				x2 /= x1;
				x2 += x1;
				x2 /= 2;
				x1 = x2;
			}
			return x2;
		}
	}

	PerlinNoise2D::PerlinNoise2D(unsigned int seed, float minVal, float maxVal, unsigned int depth)
		: PerlinNoise(seed, minVal, maxVal, depth) {}

	PerlinNoise2D::~PerlinNoise2D()
	{
	}

	typedef std::vector<std::vector<float> > GenVect;

	GenVect PerlinNoise2D::generate(size_t xn, size_t yn)
	{
		GenVect generation = GenVect(xn);
		GenVect result = GenVect(xn);
		for (size_t x = 0; x < xn; x++)
		{
			generation[x] = randomNoise(yn);
			result[x] = generation[x];
		}
		for (size_t i = 0; i < depth; i++)
		{
			GenVect nextGen = GenVect(xn);
			for (size_t x = 0; x < xn; x++)
			{
				nextGen[x] = std::vector<float>(yn);
				for (size_t y = 0; y < yn; y++)
				{
					int x1 = _perlin_noise2d::floor(x / 2.0f);
					int x2 = _perlin_noise2d::ceil(x / 2.0f);
					int y1 = _perlin_noise2d::floor(y / 2.0f);
					int y2 = _perlin_noise2d::ceil(y / 2.0f);
					nextGen[x][y] = generation[x1][y1] + generation[x2][y2];
					result[x][y] += nextGen[x][y];
				}
			}
			generation = nextGen;
		}
		return result;
	}

	std::vector<float> PerlinNoise2D::generate(size_t n)
	{
		size_t xn = (size_t)_perlin_noise2d::ceil(_perlin_noise2d::sqrt(n));
		size_t yn = xn;
		GenVect v = generate(xn, yn);
		std::vector<float> result = std::vector<float>(n);
		size_t i = 0;
		for (size_t x = 0; (x < xn) && (i < n); x++)
		{
			for (size_t y = 0; (y < yn) && (i < n); y++)
			{
				result[i++] = v[x][y];
			}
		}
		return result;
	}
}