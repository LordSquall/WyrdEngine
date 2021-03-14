#pragma once

#include "core/renderer/VertexArray.h"

namespace Osiris
{
	class OpenGLVertexArray : public VertexArray
	{
	public:
		OpenGLVertexArray();
		~OpenGLVertexArray();
		virtual void Bind() const override;
		virtual void Unbind() const override;
		virtual void SetAttribute(uint32_t id, uint32_t offset, uint32_t size, uint32_t stride) const override;

	private:
		uint32_t _rendererID;
	};
}