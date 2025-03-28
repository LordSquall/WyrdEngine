#include "wyrdpch.h"

#include "core/renderer/Renderer.h"

#include "platform/OpenGL/OpenGLTexture.h"

namespace Wyrd
{
	Texture* Texture::Create(const TextureDesc& desc)
	{
		switch (Renderer::GetAPI())
		{
		case RendererAPI::None:		
			OSR_CORE_ASSERT(false, "RendererAPI::None is currently not supported"); return nullptr;
		case RendererAPI::OpenGL:	
			return new OpenGLTexture(desc);
		default:
			OSR_CORE_ASSERT(false, "Unknown Renderer API");
		}

		return nullptr;
	}
}