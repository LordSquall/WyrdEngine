#pragma once

#include "osrpch.h"

#include "core/renderer/Renderer.h"

#include "platform/OpenGL/OpenGLTexture.h"

namespace Osiris
{
	uint32_t Texture::_nextUid = 1u;

	std::shared_ptr<Texture> Texture::Create(unsigned char* data, int width, int height, int channels, const std::string& description)
	{
		std::shared_ptr<Texture> texture;

		switch (Renderer::GetAPI())
		{
		case RendererAPI::None:		
			OSR_CORE_ASSERT(false, "RendererAPI::None is currently not supported"); return nullptr;
		case RendererAPI::OpenGL:	
			texture = std::make_shared<OpenGLTexture>(_nextUid, data, width, height, channels, description);
		default:
			OSR_CORE_ASSERT(false, "Unknown Renderer API");
		}

		_nextUid++;

		return texture;
	}
}