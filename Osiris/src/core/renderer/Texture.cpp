#pragma once

#include "osrpch.h"

#include "core/renderer/Renderer.h"

#include "platform/OpenGL/OpenGLTexture.h"

namespace Osiris
{
	Texture* Texture::Create(unsigned char* data, int width, int height, int channels, const std::string& description)
	{
		switch (Renderer::GetAPI())
		{
		case RendererAPI::None:		OSR_CORE_ASSERT(false, "RendererAPI::None is currently not supported"); return nullptr;
		case RendererAPI::OpenGL:	return new OpenGLTexture(data, width, height, channels, description);
		}

		OSR_CORE_ASSERT(false, "Unknown Renderer API");
		return nullptr;
	}
}