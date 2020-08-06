#pragma once

/* local includes */
#include "osrpch.h"
#include "core/export.h"
#include "events/Event.h"

namespace Osiris
{
	enum class RendererAPI
	{
		None = 0, OpenGL =1
	};

	enum class RendererDrawType
	{
		Triangles = 0, TriangleStrip = 1
	};

	struct RendererInfo
	{
		std::string vendor;
		std::string version;
		std::string renderer;
	};

	class OSR_LIBRARY_API Renderer
	{
	public:
		virtual ~Renderer() {}

		virtual void Clear(float r, float g, float b) const = 0;
		virtual void DrawElements(RendererDrawType type, uint32_t count) const = 0;

		static Renderer* Create();

		inline struct RendererInfo& GetVendorInfo() { return _vendorInfo; }

		inline static RendererAPI GetAPI() { return s_RendererAPI; }

	private:
		static RendererAPI s_RendererAPI;

	protected:
		struct RendererInfo _vendorInfo;
	};
}