#include "osrpch.h"
#include "OpenGLVertexArray.h"

#include <glad/glad.h>

namespace Osiris
{

	OpenGLVertexArray::OpenGLVertexArray()
	{
		glGenVertexArrays(1, &_rendererID);
		glBindVertexArray(_rendererID);
	}

	OpenGLVertexArray::~OpenGLVertexArray()
	{
	}

	void OpenGLVertexArray::Bind() const
	{
		glBindVertexArray(_rendererID);
	}

	void OpenGLVertexArray::Unbind() const
	{
		glBindVertexArray(0);
	}


	void OpenGLVertexArray::SetAttribute(uint32_t id, uint32_t offset, uint32_t size, uint32_t stride) const
	{
		glVertexAttribPointer(id, size, GL_FLOAT, GL_FALSE, stride, (void*)(offset * sizeof(float)));
		glEnableVertexAttribArray(id);
	}
}
