#include "Common.h"
#include "DefaultRenderer.h"

namespace GK
{
	void DefaultRenderer::load(int width, int height, int depth)
	{
		GLRUN(glViewport(0, 0, width, height));
		GLRUN(glEnable(GL_DEPTH_TEST));
		GLRUN(glEnable(GL_BLEND));
		GLRUN(glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA));
	}

	void DefaultRenderer::clear()
	{
		GLRUN(glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT));
	}

	DefaultRenderer::~DefaultRenderer()
	{
	}
}
