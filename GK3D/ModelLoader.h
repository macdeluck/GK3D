#ifndef _GK_MODELLOADER
#define _GK_MODELLOADER

#include "Common.h"
#include "tiny_obj_loader.h"

#include <string>
#include <vector>
#include <memory>

namespace GK
{
	struct Vertex;
	struct Material;
	class Image;

	struct ModelData {
		std::vector<Vertex> vertices;
		std::vector<GLuint> indices;
		std::shared_ptr<Material> material;
		std::shared_ptr<Image> diffuseTex;
		std::shared_ptr<Image> alphaTex;
	};

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
		
		Image * loadImage(std::string dirname, std::string filename, bool loadAlpha = false);

		void loadModel(std::string path, ModelData* modelData);
	};
}

#endif