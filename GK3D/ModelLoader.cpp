#include <memory>
#include <algorithm>

#include "ModelLoader.h"
#include "Drawable.h"
#include "Image.h"

namespace GK
{
	ModelLoader::ModelLoader() {}
	ModelLoader::~ModelLoader() {}

	#ifdef _WIN32
		const char OS_PATH_SEPARATOR = '\\';
		struct MatchPathSeparator
		{
			bool operator()( char ch ) const
			{
				return ch == '\\' || ch == '/';
			}
		};
	#elif __linux__
		const char OS_PATH_SEPARATOR = '/';
		struct MatchPathSeparator
		{
			bool operator()(char ch) const
			{
				return ch == '/';
			}
		};
	#endif

	Material* createMaterialFromMtl(tinyobj::material_t mtl)
	{
		glm::vec3 ambient(mtl.ambient[0], mtl.ambient[1], mtl.ambient[2]);
		glm::vec3 diffuse(mtl.diffuse[0], mtl.diffuse[1], mtl.diffuse[2]);
		glm::vec3 specular(mtl.specular[0], mtl.specular[1], mtl.specular[2]);
		return new Material(ambient, diffuse, specular, mtl.shininess);
	}

	Image* ModelLoader::loadImage(std::string dirname, std::string filename)
	{
		Image* result = new Image();
		result->loadFile(dirname + filename);
		return result;
	}

	void splitPath(std::string const& pathname, std::string* basename, std::string* directory)
	{
		std::string::const_reverse_iterator it = std::find_if(pathname.rbegin(), pathname.rend(), MatchPathSeparator());
		(*basename) = std::string(it.base(), pathname.end());
		(*directory) = std::string(pathname.begin(), it.base());
	}

	void ModelLoader::loadModel(std::string path, ModelData* modelData)
	{
		std::string basename, directory;
		splitPath(path, &basename, &directory);

		std::vector<tinyobj::shape_t> shapes;
		std::vector<tinyobj::material_t> materials;
		std::string err = tinyobj::LoadObj(shapes, materials, path.c_str(), directory.c_str());

		modelData->vertices.clear();
		modelData->indices.clear();
		modelData->material.reset();
		modelData->diffuseTex.reset();
		modelData->alphaTex.reset();

		if (materials.size() > 0)
		{
			modelData->material.reset(createMaterialFromMtl(*materials.begin()));
			if (!materials.begin()->diffuse_texname.empty())
				modelData->diffuseTex.reset(loadImage(directory, materials.begin()->diffuse_texname));
			else modelData->diffuseTex.reset();
			if (!materials.begin()->alpha_texname.empty())
				modelData->alphaTex.reset(loadImage(directory, materials.begin()->alpha_texname));
			else modelData->alphaTex.reset();
		}
		if (!err.empty())
			throw Exception(err);
		loadShapes(shapes, &modelData->vertices, &modelData->indices);
	}

	typedef std::vector<std::shared_ptr<std::vector<GLfloat> > > FaceVertexData;

	void ModelLoader::loadShapes(std::vector<tinyobj::shape_t> shapes,
		std::vector<Vertex>* output,
		std::vector<GLuint>* indices)
	{
		for (size_t i = 0; i < shapes.size(); i++)
		{
			if ((shapes[i].mesh.positions.size() % Vertex::VERTEX_POSITIONS) != 0)
				throw Exception("Vertex stride is different than " + std::to_string(Vertex::VERTEX_POSITIONS));
			if ((shapes[i].mesh.indices.size() % Vertex::VERTEX_POSITIONS) != 0)
				throw Exception("Indices stride is different than " + std::to_string(Vertex::VERTEX_POSITIONS));

			bool calculateNormals = shapes[i].mesh.normals.empty(); 
			calculateNormals |= (shapes[i].mesh.normals.size() != shapes[i].mesh.positions.size());

			size_t vertexNum = shapes[i].mesh.positions.size() / Vertex::VERTEX_POSITIONS;
			size_t triangesNum = shapes[i].mesh.indices.size() / Vertex::VERTEX_POSITIONS;
			size_t lastVertexIndex = output->size();
			output->reserve(output->size() + vertexNum);
			indices->reserve(indices->size() + shapes[i].mesh.indices.size());

			// load face vertexes
			FaceVertexData faceData = FaceVertexData(vertexNum);
			for (size_t vertexNo = 0; vertexNo < vertexNum; vertexNo++)
			{
				faceData[vertexNo].reset(new std::vector<GLfloat>(Vertex::VERTEX_SIZE));
				for (size_t j = 0; j < Vertex::VERTEX_POSITIONS; j++)
				{
					(*faceData[vertexNo])[j] = shapes[i].mesh.positions[vertexNo * Vertex::VERTEX_POSITIONS + j];
				}
				for (size_t j = 0; j < Vertex::VERTEX_TEXCOORDS; j++)
				{
					if (shapes[i].mesh.texcoords.size() > 0)
						(*faceData[vertexNo])[Vertex::VERTEX_POSITIONS + Vertex::VERTEX_NORMALS + j] = shapes[i].mesh.texcoords[vertexNo * Vertex::VERTEX_TEXCOORDS + j];
					else (*faceData[vertexNo])[Vertex::VERTEX_POSITIONS + Vertex::VERTEX_NORMALS + j] = -1;
				}
			}

			// load face normal
			if (calculateNormals)
			{
				for (size_t triangleNo = 0; triangleNo < triangesNum; triangleNo++)
				{
					calculateAndSetNormal(
						faceData[shapes[i].mesh.indices[triangleNo * Vertex::VERTEX_POSITIONS + 0]],
						faceData[shapes[i].mesh.indices[triangleNo * Vertex::VERTEX_POSITIONS + 1]],
						faceData[shapes[i].mesh.indices[triangleNo * Vertex::VERTEX_POSITIONS + 2]]);
				}
			}
			else
			{
				for (size_t vertexNo = 0; vertexNo < vertexNum; vertexNo++)
				{
					for (size_t j = 0; j < Vertex::VERTEX_POSITIONS; j++)
					{
						(*faceData[vertexNo])[j + Vertex::VERTEX_POSITIONS] = shapes[i].mesh.normals[vertexNo * Vertex::VERTEX_POSITIONS + j];
					}
				}
			}

			// flush indices and vertexes
			for (size_t j = 0; j < shapes[i].mesh.indices.size(); j++)
			{
				indices->push_back(lastVertexIndex + shapes[i].mesh.indices[j]);
			}

			for (size_t j = 0; j < faceData.size(); j++)
			{
				output->push_back(Vertex((*faceData[j]).data()));
			}
		}
	}

	void ModelLoader::calculateAndSetNormal(
		std::shared_ptr<std::vector<GLfloat> > vertex1,
		std::shared_ptr<std::vector<GLfloat> > vertex2,
		std::shared_ptr<std::vector<GLfloat> > vertex3)
	{
		glm::vec3 u = glm::vec3(
			(*vertex2)[0] - (*vertex1)[0],
			(*vertex2)[1] - (*vertex1)[1],
			(*vertex2)[2] - (*vertex1)[2]);
		glm::vec3 v = glm::vec3(
			(*vertex3)[0] - (*vertex1)[0],
			(*vertex3)[1] - (*vertex1)[1],
			(*vertex3)[2] - (*vertex1)[2]);
		glm::vec3 normal = glm::normalize(glm::vec3(
			u.y*v.z - u.z*v.y,
			u.z*v.x - u.x*v.z,
			u.x*v.y - u.y*v.x));
		(*vertex1)[Vertex::VERTEX_POSITIONS + 0] = (*vertex2)[Vertex::VERTEX_POSITIONS + 0] = (*vertex3)[Vertex::VERTEX_POSITIONS + 0] = normal.x;
		(*vertex1)[Vertex::VERTEX_POSITIONS + 1] = (*vertex2)[Vertex::VERTEX_POSITIONS + 1] = (*vertex3)[Vertex::VERTEX_POSITIONS + 1] = normal.y;
		(*vertex1)[Vertex::VERTEX_POSITIONS + 2] = (*vertex2)[Vertex::VERTEX_POSITIONS + 2] = (*vertex3)[Vertex::VERTEX_POSITIONS + 2] = normal.z;
	}
}