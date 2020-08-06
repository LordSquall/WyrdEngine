#include "osrpch.h"
#include "OpenGLBuffer.h"
#include "OpenGLError.h"

#include <glad/glad.h>

namespace Osiris
{

	OpenGLVertexBuffer::OpenGLVertexBuffer(float* vertices, uint32_t size, const std::string& description)
	{
		glCreateBuffers(1, &_rendererID);
		glBindBuffer(GL_ARRAY_BUFFER, _rendererID);
		//glObjectLabel(GL_BUFFER, _rendererID, -1, description.c_str());
		glBufferData(GL_ARRAY_BUFFER, size, vertices, GL_STATIC_DRAW);

		GLenum err = glGetError();

		if (err != GL_NO_ERROR)
			OpenGLError::Resolve(err);

	}

	OpenGLVertexBuffer::~OpenGLVertexBuffer()
	{
		glDeleteBuffers(1, &_rendererID);
	}

	void OpenGLVertexBuffer::Bind() const
	{
		glBindBuffer(GL_ARRAY_BUFFER, _rendererID);
	}

	void OpenGLVertexBuffer::Unbind() const
	{
		glBindBuffer(GL_ARRAY_BUFFER, 0);
	}

	void OpenGLVertexBuffer::Update(float* vertices, uint32_t size, uint32_t offset) const
	{
		glBindBuffer(GL_ARRAY_BUFFER, _rendererID);
		glBufferData(GL_ARRAY_BUFFER, size, vertices, GL_STATIC_DRAW);

		GLenum err = glGetError();

		if (err != GL_NO_ERROR)
			OpenGLError::Resolve(err);

	}

	OpenGLIndexBuffer::OpenGLIndexBuffer(uint32_t* indices, uint32_t count)
		: _count(count)
	{
		glCreateBuffers(1, &_rendererID);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, _rendererID);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, count * sizeof(uint32_t), indices, GL_STATIC_DRAW);
	}

	OpenGLIndexBuffer::~OpenGLIndexBuffer()
	{
		glDeleteBuffers(1, &_rendererID);
	}


	void OpenGLIndexBuffer::Bind() const
	{
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, _rendererID);
	}

	void OpenGLIndexBuffer::Unbind() const
	{
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	}

	void OpenGLIndexBuffer::Update(uint32_t* indices, uint32_t count)
	{
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, _rendererID);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, count * sizeof(uint32_t), indices, GL_STATIC_DRAW);

		_count = count;

		GLenum err = glGetError();

		if (err != GL_NO_ERROR)
			OSR_CORE_ERROR(err);
	}

	OpenGLFrameBuffer::OpenGLFrameBuffer(const FrameBufferConfig& config)
	{
		_config = config;

		Build();
	}

	OpenGLFrameBuffer::~OpenGLFrameBuffer()
	{
		glDeleteFramebuffers(1, &_rendererID);
		glDeleteTextures(1, &_colorAttachmentID);
		glDeleteTextures(1, &_depthStencilAttachmentID);
	}

	void OpenGLFrameBuffer::Build()
	{
		if (_rendererID != 0)
		{
			glDeleteFramebuffers(1, &_rendererID);
			glDeleteTextures(1, &_colorAttachmentID);
			glDeleteTextures(1, &_depthStencilAttachmentID);
		}

		glGenFramebuffers(1, &_rendererID);
		glBindFramebuffer(GL_FRAMEBUFFER, _rendererID);

		/* Create the color texture attachment */
		glGenTextures(1, &_colorAttachmentID);
		glBindTexture(GL_TEXTURE_2D, _colorAttachmentID);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, _config.width, _config.height, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

		/* Attach color texture to framebuffer */
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, _colorAttachmentID, 0);

		/* Create the depth/stencil texture */
		glGenTextures(1, &_depthStencilAttachmentID);
		glBindTexture(GL_TEXTURE_2D, _depthStencilAttachmentID);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH24_STENCIL8, _config.width, _config.height, 0, GL_DEPTH_STENCIL, GL_UNSIGNED_INT_24_8, NULL);

		/* Attach depth/stencil to framebuffer */
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_TEXTURE_2D, _depthStencilAttachmentID, 0);

		/* Create render buffer attachment */
		glGenRenderbuffers(1, &_renderBufferAttachmentID);
		glBindRenderbuffer(GL_RENDERBUFFER, _renderBufferAttachmentID);

		/* Allocated the storage on the GPU for the render buffer data */
		glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, _config.width, _config.height);

		/* Attach renderbuffer to framebuffer */
		glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, _renderBufferAttachmentID);

		/* Check framebuffer status */
		if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
		{
			OSR_CORE_ERROR("Framebuffer is not complete!");
		}

		/* Clear the current framebuffer binding */
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
	}

	void OpenGLFrameBuffer::Bind() const
	{
		glBindFramebuffer(GL_FRAMEBUFFER, _rendererID);

		glViewport(0, 0, _config.width, _config.height);
	}

	void OpenGLFrameBuffer::Unbind() const
	{
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
	}

	void OpenGLFrameBuffer::Resize(uint32_t width, uint32_t height)
	{
		/* update the configuration */
		_config.width = width;
		_config.height = height;

		/* re-build framebuffer */
		Build();
	}


	uint32_t OpenGLFrameBuffer::GetColorAttachmentID() const
	{
		return _colorAttachmentID;
	}

	const FrameBufferConfig& OpenGLFrameBuffer::GetConfig() const
	{
		return _config;
	}
}
