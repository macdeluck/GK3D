#ifndef _GK_SHADER
#define _GK_SHADER

#include "Common.h"

namespace GK
{
	class ShaderProgram;
	struct DrawableInstance;

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
		void use();
		virtual void before_link();
		virtual void update();
		virtual void beforeRender(DrawableInstance drawable);
	protected:
		GLuint getUniformLocation(std::string uniformName);
		GLuint getProgramId();
	private:
		bool compiled;
		std::shared_ptr<GLuint> programId;
		std::shared_ptr<Shader> vertexShader;
		std::shared_ptr<Shader> fragmentShader;
	};
}

#endif