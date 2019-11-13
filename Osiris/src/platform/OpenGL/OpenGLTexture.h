#pragma once

#include "core/renderer/Texture.h"

namespace Osiris
{
	class OpenGLTexture : public Texture
	{
	public:
		OpenGLTexture(unsigned char* data, int width, int height, int channels, const std::string& description);
		~OpenGLTexture();
		virtual void Bind() const override;
		virtual void Unbind() const override;
		virtual uint32_t GetHandle() const override;

	private:
		uint32_t _rendererID;
	};
}