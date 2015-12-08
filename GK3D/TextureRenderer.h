#ifndef _GK_TEXTURERENDERER
#define _GK_TEXTURERENDERER

#include "IRenderer.h"
#include <memory>

namespace GK
{
	struct Texture;

	class TextureRenderer : public IRenderer
	{
	protected:
		std::shared_ptr<unsigned int> frameBuffer;
		std::shared_ptr<unsigned int> renderBuffer;
		std::shared_ptr<Texture> targetTexture;
	public:
		TextureRenderer(int width, int height, int depth);
		virtual ~TextureRenderer();

		void load(int width, int height, int depth);
		void lightLoad();
		void clear();
	};
}

#endif