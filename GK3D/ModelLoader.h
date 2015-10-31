#ifndef _GK_MODELLOADER
#define _GK_MODELLOADER

#include "Common.h"
#include "tiny_obj_loader.h"

#include <string>
#include <vector>

namespace GK
{
	struct Vertex;

	class ModelLoader
	{
	private:
		void loadShapes(std::vector<tinyobj::shape_t> shapes,
			std::vector<Vertex>* vertices,
			std::vector<GLuint>* indices);
		void calculateAndSetNormal(
			std::shared_ptr<std::vector<GLfloat> > vertex1,
			std::shared_ptr<std::vector<GLfloat> > vertex2,
			std::shared_ptr<std::vector<GLfloat> > vertex3);

	public:
		ModelLoader();
		virtual ~ModelLoader();
		
		void loadModel(std::string path,
			std::vector<Vertex>* vertices,
			std::vector<GLuint>* indices);
	};
}

#endif