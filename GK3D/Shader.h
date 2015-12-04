#ifndef _GK_SHADER
#define _GK_SHADER

#include "Common.h"

namespace GK
{
	class ShaderProgram;
	class Scene;
	class DrawableInstance;

	enum ShaderType
	{
		Unknown = 0,
		VertexShader = GL_VERTEX_SHADER,
		FragmentShader = GL_FRAGMENT_SHADER
	};

	class Shader
	{
	public:
		Shader(std::string source, ShaderType type);
		virtual ~Shader();

		virtual ShaderType type();
		static std::string FromFile(std::string path);
	protected:
		GLuint getShaderId();
	private:
		std::shared_ptr<GLuint> shaderId;
		ShaderType shaderType;
		friend class ShaderProgram;
	};

	class ShaderProgram
	{
	public:
		ShaderProgram(std::shared_ptr<Shader> vertexShader, std::shared_ptr<Shader> fragmentShader);
		virtual ~ShaderProgram();
		std::shared_ptr<Shader> getVertexShader();
		std::shared_ptr<Shader> getFragmentShader();
		void compile();
		virtual void beforeLink();
		virtual void postRender(std::shared_ptr<DrawableInstance> drawable, std::shared_ptr<Scene> scene);
		virtual void preRender(std::shared_ptr<DrawableInstance> drawable, std::shared_ptr<Scene> scene);
		virtual void doRender(std::shared_ptr<DrawableInstance> drawable, std::shared_ptr<Scene> scene);
	protected:
		virtual void render(std::shared_ptr<DrawableInstance> drawable, std::shared_ptr<Scene> scene);
		void bindTexture(std::string uniformName, GLuint textureID, GLenum textureType = GL_TEXTURE_2D);
		GLuint getUniformLocation(std::string uniformName);
		GLuint getProgramId();
	private:
		void use();
		bool compiled;
		GLuint currentTextureUnit;
		std::shared_ptr<GLuint> programId;
		std::shared_ptr<Shader> vertexShader;
		std::shared_ptr<Shader> fragmentShader;

		void clearTextureUnits();
	};
}

#endif