#include "osrpch.h"
#include "VertexArray.h"

#include "core/renderer/Renderer.h"

#include "platform/OpenGL/OpenGLVertexArray.h"

namespace Osiris
{

	VertexArray* VertexArray::Create()
	{
		switch (Renderer::GetAPI())
		{
		case RendererAPI::None:		OSR_CORE_ASSERT(false, "RendererAPI::None is currently not supported"); return nullptr;
		case RendererAPI::OpenGL:	return new OpenGLVertexArray();
		}

		OSR_CORE_ASSERT(false, "Unknown Renderer API");
		return nullptr;
	}
}