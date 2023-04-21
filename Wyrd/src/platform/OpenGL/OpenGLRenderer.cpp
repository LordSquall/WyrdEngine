#include "wyrdpch.h"
#include "OpenGLRenderer.h"
#include "core/Log.h"

#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glad/glad.h>

namespace Wyrd
{
	OpenGLRenderer::OpenGLRenderer()
	{
		std::ostringstream stringStream;
		stringStream << glGetString(GL_VENDOR);
		_vendorInfo.vendor = stringStream.str();
		stringStream.str(std::string());

		stringStream << glGetString(GL_VERSION);
		_vendorInfo.version = stringStream.str();
		stringStream.str(std::string());

		stringStream << glGetString(GL_RENDERER);
		_vendorInfo.renderer = stringStream.str();
		stringStream.str(std::string());

		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		
		glEnable(GL_DEPTH_TEST);
	}

	OpenGLRenderer::~OpenGLRenderer()
	{
	}


	void OpenGLRenderer::SetViewport(float width, float height)
	{
		glViewport(0, 0, width, height);
	}

	void OpenGLRenderer::Clear(float r, float g, float b) const
	{
		glClearColor(r, g, b, 1);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
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
		case RendererDrawType::TriangleFan:
			primitiveType = GL_TRIANGLE_FAN;
			break;
		}

		glDrawElements(primitiveType, count, GL_UNSIGNED_INT, nullptr);
	}

	void OpenGLRenderer::DrawArray(RendererDrawType type, uint32_t offset, uint32_t count) const
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
		case RendererDrawType::TriangleFan:
			primitiveType = GL_TRIANGLE_FAN;
			break;
		case RendererDrawType::Quads:
			primitiveType = GL_QUADS;
			break;
		}

		glDrawArrays(primitiveType, offset, count);
	}


#ifdef WYRD_INCLUDE_DEBUG_TAGS

	void OpenGLRenderer::StartNamedSection(const char* name) const
	{
		glPushDebugGroup(GL_DEBUG_SOURCE_APPLICATION, 0, -1, name);
	}

	void OpenGLRenderer::EndNamedSection() const
	{
		glPopDebugGroup();
	}

#endif
}
