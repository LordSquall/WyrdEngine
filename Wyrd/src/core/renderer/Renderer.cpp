#include "wyrdpch.h"
#include "core/renderer/Renderer.h"

#include "platform/OpenGL/OpenGLRenderer.h"

namespace Wyrd
{
	RendererAPI Renderer::s_RendererAPI = RendererAPI::OpenGL;

	Renderer* Renderer::Create()
	{
		switch (Renderer::GetAPI())
		{
		case RendererAPI::None:		OSR_CORE_ASSERT(false, "RendererAPI::None is currently not supported"); return nullptr;
		case RendererAPI::OpenGL:	return new OpenGLRenderer();
		}

		OSR_CORE_ASSERT(false, "Unknown Renderer API");
		return nullptr;
	}
}