#include "osrpch.h"
#include "OpenGLRenderer.h"

#include <glad/glad.h>

namespace Osiris
{
	OpenGLRenderer::OpenGLRenderer()
	{
	}

	OpenGLRenderer::~OpenGLRenderer()
	{
	}

	void OpenGLRenderer::Clear(float r, float g, float b) const
	{
		glClearColor(r, g, b, 1);
		glClear(GL_COLOR_BUFFER_BIT);
	}

	void OpenGLRenderer::DrawElements(RendererDrawType type, uint32_t count) const
	{
		GLuint primitiveType = 0;

		/* map renderer primitive to opengl primitive */
		switch (type)
		{
		case RendererDrawType::Triangles:
			primitiveType = GL_TRIANGLES;
			break;
		case RendererDrawType::TriangleStrip:
			primitiveType = GL_TRIANGLE_STRIP;
			break;
		}

		glDrawElements(primitiveType, count, GL_UNSIGNED_INT, nullptr);
	}
}
