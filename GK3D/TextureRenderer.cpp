#include "TextureRenderer.h"
#include "Common.h"
#include "Texture.h"

namespace GK
{
	void TextureRenderer::load(int width, int height, int depth)
	{
		GLRUN(glBindFramebuffer(GL_FRAMEBUFFER, *frameBuffer));
	}

	void TextureRenderer::clear()
	{
		GLRUN(glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT));
	}

	Texture generateAttachmentTexture(int width, int height)
	{
		// What enum to use?
		GLenum attachment_type = GL_DEPTH_COMPONENT;

		//Generate texture ID and load texture data 
		GLuint textureID;
		GLRUN(glGenTextures(1, &textureID));
		GLRUN(glBindTexture(GL_TEXTURE_2D, textureID));
		GLRUN(glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH24_STENCIL8, width, height, 0, GL_DEPTH_STENCIL, GL_UNSIGNED_INT_24_8, NULL));
		GLRUN(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR));
		GLRUN(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR));
		GLRUN(glBindTexture(GL_TEXTURE_2D, 0));

		return Texture(textureID);
	}

	TextureRenderer::TextureRenderer(int width, int height, int depth)
		: frameBuffer(new GLuint), renderBuffer(new GLuint)
	{
		GLRUN(glGenFramebuffers(1, &(*frameBuffer)));
		GLRUN(glBindFramebuffer(GL_FRAMEBUFFER, *frameBuffer));

		targetTexture = std::shared_ptr<Texture>(new Texture(generateAttachmentTexture(width, height)));
		GLRUN(glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, targetTexture->getId(), 0));

		GLRUN(glGenRenderbuffers(1, &(*renderBuffer)));
		GLRUN(glBindRenderbuffer(GL_RENDERBUFFER, *renderBuffer));
		GLRUN(glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, width, height));
		GLRUN(glBindRenderbuffer(GL_RENDERBUFFER, 0));
		GLRUN(glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, *renderBuffer));

		GLenum frameBufferStatus;
		GLRUNR(glCheckFramebufferStatus(GL_FRAMEBUFFER), frameBufferStatus);
		if (frameBufferStatus != GL_FRAMEBUFFER_COMPLETE)
			throw Exception("Incomplete frame buffer: " + std::to_string(frameBufferStatus));
		GLRUN(glBindFramebuffer(GL_FRAMEBUFFER, 0));
	}

	TextureRenderer::~TextureRenderer()
	{
		if (frameBuffer.unique())
			glDeleteFramebuffers(1, &(*frameBuffer));
		if (renderBuffer.unique())
			glDeleteRenderbuffers(1, &(*renderBuffer));
	}
}