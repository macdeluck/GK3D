#include <memory>
#include <algorithm>

#include "ModelLoader.h"
#include "Drawable.h"

namespace GK
{
	ModelLoader::ModelLoader() {}
	ModelLoader::~ModelLoader() {}

	#ifdef _WIN32
		struct MatchPathSeparator
		{
			bool operator()( char ch ) const
			{
				return ch == '\\' || ch == '/';
			}
		};
	#elif __linux__
		struct MatchPathSeparator
		{
			bool operator()(char ch) const
			{
				return ch == '/';
			}
		};
	#endif

	void splitPath(std::string const& pathname, std::string* basename, std::string* directory)
	{
		std::string::const_reverse_iterator it = std::find_if(pathname.rbegin(), pathname.rend(), MatchPathSeparator());
		(*basename) = std::string(it.base(), pathname.end());
		(*directory) = std::string(pathname.begin(), it.base());
	}

	void ModelLoader::loadModel(std::string path, 
		std::vector<Vertex>* output, 
		std::vector<GLuint>* indices)
	{
		std::string basename, directory;
		splitPath(path, &basename, &directory);

		std::vector<tinyobj::shape_t> shapes;
		std::vector<tinyobj::material_t> materials;
		std::string err = tinyobj::LoadObj(shapes, materials, path.c_str(), directory.c_str());
		if (!err.empty())
			throw Exception(err);
		loadShapes(shapes, output, indices);
	}

	typedef std::vector<std::shared_ptr<std::vector<GLfloat> > > FaceVertexData;

	void ModelLoader::loadShapes(std::vector<tinyobj::shape_t> shapes,
		std::vector<Vertex>* output,
		std::vector<GLuint>* indices)
	{
		const size_t vertexStride = 3;

		for (size_t i = 0; i < shapes.size(); i++)
		{
			if ((shapes[i].mesh.positions.size() % vertexStride) != 0)
				throw Exception("Vertex stride is different than " + std::to_string(vertexStride));
			if ((shapes[i].mesh.indices.size() % vertexStride) != 0)
				throw Exception("Indices stride is different than " + std::to_string(vertexStride));

			bool calculateNormals = shapes[i].mesh.normals.empty(); 
			calculateNormals |= (shapes[i].mesh.normals.size() != shapes[i].mesh.positions.size());

			size_t vertexNum = shapes[i].mesh.positions.size() / vertexStride;
			size_t triangesNum = shapes[i].mesh.indices.size() / vertexStride;
			size_t lastVertexIndex = output->size();
			output->reserve(output->size() + vertexNum);
			indices->reserve(indices->size() + shapes[i].mesh.indices.size());

			// load face vertexes
			FaceVertexData faceData = FaceVertexData(vertexNum);
			for (size_t vertexNo = 0; vertexNo < vertexNum; vertexNo++)
			{
				faceData[vertexNo].reset(new std::vector<GLfloat>(Vertex::VERTEX_SIZE));
				for (size_t j = 0; j < vertexStride; j++)
				{
					(*faceData[vertexNo])[j] = shapes[i].mesh.positions[vertexNo * vertexStride + j];
				}
			}

			// load face normal
			if (calculateNormals)
			{
				for (size_t triangleNo = 0; triangleNo < triangesNum; triangleNo++)
				{
					calculateAndSetNormal(
						faceData[shapes[i].mesh.indices[triangleNo * vertexStride + 0]],
						faceData[shapes[i].mesh.indices[triangleNo * vertexStride + 1]],
						faceData[shapes[i].mesh.indices[triangleNo * vertexStride + 2]]);
				}
			}
			else
			{
				for (size_t vertexNo = 0; vertexNo < vertexNum; vertexNo++)
				{
					for (size_t j = 0; j < vertexStride; j++)
					{
						(*faceData[vertexNo])[j + vertexStride] = shapes[i].mesh.normals[vertexNo * vertexStride + j];
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
		(*vertex1)[3] = (*vertex2)[3] = (*vertex3)[3] = normal.x;
		(*vertex1)[4] = (*vertex2)[4] = (*vertex3)[4] = normal.y;
		(*vertex1)[5] = (*vertex2)[5] = (*vertex3)[5] = normal.z;
	}
}