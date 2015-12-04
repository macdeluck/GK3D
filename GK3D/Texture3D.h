#ifndef _GK_TEXTURE3D
#define _GK_TEXTURE3D

#include "Common.h"
#include <memory>
#include <map>

namespace GK
{
	struct Image;

	enum Texture3DFace
	{
		Texture3DFaceFront = 5,
		Texture3DFaceBack = 4,
		Texture3DFaceLeft = 1,
		Texture3DFaceRight = 0,
		Texture3DFaceTop = 2,
		Texture3DFaceBottom = 3
	};

	struct Texture3D
	{
	private:
		std::shared_ptr<GLuint> texture;

	public:
		Texture3D();
		Texture3D(std::map<Texture3DFace, std::shared_ptr<Image> > faces);
		virtual ~Texture3D();
		bool empty();
		GLuint getId();

		friend class Drawable;
	};
}

#endif