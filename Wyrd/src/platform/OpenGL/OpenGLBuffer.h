#pragma once

#include "core/renderer/Buffer.h"

namespace Wyrd
{
	class OpenGLVertexBuffer : public VertexBuffer
	{
	public:
		OpenGLVertexBuffer(float* vertices, size_t size, const std::string& description);
		~OpenGLVertexBuffer();
		virtual void Bind() const override;
		virtual void Unbind() const override;
		virtual void Update(float* vertices, size_t size, uint32_t offset) const override;

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

	class OpenGLFrameBuffer : public FrameBuffer
	{
	public:
		OpenGLFrameBuffer(const FrameBufferConfig& config);
		~OpenGLFrameBuffer();

		void Build();

		virtual void Bind() const override;
		virtual void Unbind() const override;

		virtual void Resize(uint32_t width, uint32_t height) override;

		virtual uint32_t GetColorAttachmentID() const override;
		virtual const FrameBufferConfig& GetConfig() const override;

	private:
		FrameBufferConfig _config;

		uint32_t _rendererID;
		uint32_t _colorAttachmentID;
		uint32_t _depthStencilAttachmentID;
		uint32_t _renderBufferAttachmentID;
	};
}