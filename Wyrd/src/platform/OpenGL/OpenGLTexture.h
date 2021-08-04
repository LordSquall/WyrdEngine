#pragma once

#include "core/renderer/Texture.h"

namespace Wyrd
{
	class OpenGLTexture : public Texture
	{
	public:
		OpenGLTexture(const TextureDesc& desc);
		~OpenGLTexture();
		virtual void Bind() const override;
		virtual void Unbind() const override;
		virtual uint32_t GetHandle() const override;
		virtual void Update(unsigned char* data, int xOffset, int yOffset, int width, int height) override;
		virtual void Delete() override;

	private:
		uint32_t _rendererID;
	};
}