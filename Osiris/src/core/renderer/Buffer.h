#pragma once

#include <string>

namespace Osiris
{
	class VertexBuffer
	{
	public:
		virtual ~VertexBuffer() {}

		virtual void Bind() const = 0;
		virtual void Unbind() const = 0;
		virtual void Update(float* vertices, uint32_t size, uint32_t offset) const = 0;

		static VertexBuffer* Create(float* vertices, uint32_t size, const std::string& description);
	};

	class IndexBuffer
	{
	public:
		virtual ~IndexBuffer() {}

		virtual void Bind() const = 0;
		virtual void Unbind() const = 0;
		virtual void Update(uint32_t* indices, uint32_t size) = 0;

		virtual uint32_t GetCount() const = 0;

		static IndexBuffer* Create(uint32_t* indices, uint32_t size);
	};

	struct FrameBufferConfig
	{
		uint32_t width = 800;
		uint32_t height = 600;
	};

	class FrameBuffer
	{
	public:
		virtual ~FrameBuffer() {}

		virtual void Bind() const = 0;
		virtual void Unbind() const = 0;

		virtual void Resize(uint32_t width, uint32_t height) = 0;

		virtual uint32_t GetColorAttachmentID() const = 0;
		virtual const FrameBufferConfig& GetConfig() const = 0;

		static FrameBuffer* Create(const FrameBufferConfig& config);
	};
}