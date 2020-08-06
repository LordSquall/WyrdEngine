#pragma once

#include "core/renderer/Texture.h"

namespace Osiris
{
	class OpenGLTexture : public Texture
	{
	public:
		OpenGLTexture(uint32_t uid, unsigned char* data, int width, int height, int channels, const std::string& description);
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