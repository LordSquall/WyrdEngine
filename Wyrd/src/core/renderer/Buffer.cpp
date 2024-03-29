#include "wyrdpch.h"
#include "Buffer.h"

#include "core/renderer/Renderer.h"

#include "platform/OpenGL/OpenGLBuffer.h"

namespace Wyrd
{

	VertexBuffer* VertexBuffer::Create(float* vertices, size_t size, const std::string& description)
	{
		switch (Renderer::GetAPI())
		{
		case RendererAPI::None:		OSR_CORE_ASSERT(false, "RendererAPI::None is currently not supported"); return nullptr;
		case RendererAPI::OpenGL:	return new OpenGLVertexBuffer(vertices, size, description);
		}

		OSR_CORE_ASSERT(false, "Unknown Renderer API");
		return nullptr;
	}

	IndexBuffer* IndexBuffer::Create(uint32_t* indices, uint32_t count)
	{
		switch (Renderer::GetAPI())
		{
		case RendererAPI::None:		OSR_CORE_ASSERT(false, "RendererAPI::None is currently not supported"); return nullptr;
		case RendererAPI::OpenGL:	return new OpenGLIndexBuffer(indices, count);
		}

		OSR_CORE_ASSERT(false, "Unknown Renderer API");
		return nullptr;
	}

	FrameBuffer* FrameBuffer::Create(const FrameBufferConfig& config)
	{
		switch (Renderer::GetAPI())
		{
		case RendererAPI::None:		OSR_CORE_ASSERT(false, "RendererAPI::None is currently not supported"); return nullptr;
		case RendererAPI::OpenGL:	return new OpenGLFrameBuffer(config);
		}

		OSR_CORE_ASSERT(false, "Unknown Renderer API");
		return nullptr;
	}
}