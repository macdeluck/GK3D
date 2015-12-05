#ifndef _GK_NOISE
#define _GK_NOISE

#include <cstdlib>
#include <vector>

namespace GK
{
	class INoise
	{
	public:
		INoise();
		virtual ~INoise();
		virtual std::vector<float> generate(size_t n) = 0;
		std::vector<float> operator()(size_t n);
	};
}

#endif