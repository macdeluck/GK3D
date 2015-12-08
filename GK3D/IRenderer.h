#ifndef _GK_IRENDERER
#define _GK_IRENDERER

namespace GK
{
	class IRenderer
	{
	public:
		virtual void load(int width, int height, int depth) = 0;
		virtual void lightLoad() = 0;
		virtual void clear() = 0;
		virtual ~IRenderer() {}
	};
}

#endif