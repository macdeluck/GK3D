#ifndef _GK_DEFAULTRENDERER
#define _GK_DEFAULTRENDERER

#include "IRenderer.h"

namespace GK
{
	class DefaultRenderer : public IRenderer
	{
	public:
		void load(int width, int height, int depth);
		void clear();
		virtual ~DefaultRenderer();
	};
}

#endif