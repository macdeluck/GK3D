#include "Common.h"
#include "DefaultRenderer.h"

namespace GK
{
	void DefaultRenderer::load(int width, int height, int depth)
	{
		GLRUN(glViewport(0, 0, width, height));
		lightLoad();
	}

	void DefaultRenderer::lightLoad()
	{
		GLRUN(glBindFramebuffer(GL_FRAMEBUFFER, 0));
	}

	DefaultRenderer::~DefaultRenderer()
	{
	}
}