#ifndef _GK_TEXTURERENDERER
#define _GK_TEXTURERENDERER

#include "IRenderer.h"
#include "Texture.h"

namespace GK
{
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
		Texture getTexture();
	};
}

#endif