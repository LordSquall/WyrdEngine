#include "osrpch.h"
#include "core/renderer/Renderer.h"

#include "platform/OpenGL/OpenGLShader.h"

namespace Osiris
{
	Shader* Shader::Create()
	{
		switch (Renderer::GetAPI())
		{
		case RendererAPI::None:		OSR_CORE_ASSERT(false, "RendererAPI::None is currently not supported"); return nullptr;
		case RendererAPI::OpenGL:	return new OpenGLShader();
		}

		OSR_CORE_ASSERT(false, "Unknown Renderer API");
		return nullptr;
	}
}