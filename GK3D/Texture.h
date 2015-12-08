#ifndef _GK_TEXTURE
#define _GK_TEXTURE

#include "Common.h"

namespace GK
{
	class Image;
	class Drawable;

	struct Texture
	{
	private:
		std::shared_ptr<GLuint> texture;

	public:
		Texture();
		Texture(GLuint textureId);
		Texture(std::shared_ptr<Image> image);
		virtual ~Texture();
		bool empty();
		GLuint getId();

		friend class Drawable;
	};
}

#endif