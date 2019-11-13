#pragma once

#include "core/renderer/Buffer.h"

namespace Osiris
{
	class OpenGLVertexBuffer : public VertexBuffer
	{
	public:
		OpenGLVertexBuffer(float* vertices, uint32_t size, const std::string& description);
		~OpenGLVertexBuffer();
		virtual void Bind() const override;
		virtual void Unbind() const override;
		virtual void Update(float* vertices, uint32_t size, uint32_t offset) const override;

	private:
		uint32_t _rendererID;
	};

	class OpenGLIndexBuffer : public IndexBuffer
	{
	public:
		OpenGLIndexBuffer(uint32_t* indices, uint32_t count);
		~OpenGLIndexBuffer();
		virtual void Bind() const override;
		virtual void Unbind() const override;
		virtual void Update(uint32_t* indices, uint32_t count) override;

		virtual uint32_t GetCount() const { return _count; }

	private:
		uint32_t _rendererID;
		uint32_t _count;
	};
}