#ifndef _GK_TEXTURE3D
#define _GK_TEXTURE3D

#include "Common.h"
#include "Face3D.h"
#include <memory>
#include <map>

namespace GK
{
	class Image;

	struct Texture3D
	{
	private:
		std::shared_ptr<GLuint> texture;

	public:
		Texture3D();
		Texture3D(std::map<Face3D, std::shared_ptr<Image> > faces);
		virtual ~Texture3D();
		bool empty();
		GLuint getId();

		friend class Drawable;
	};
}

#endif