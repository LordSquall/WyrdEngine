#pragma once

/* local includes */
#include "wyrdpch.h"
#include "core/export.h"
#include "core/maths/Rect.h"
#include "events/Event.h"
#include "core/renderer/commands/RendererCommands.h"
#include "core/pipeline/Vertex2DBatch.h"
#include "core/pipeline/SpriteBatch.h"

namespace Wyrd
{
	enum class RendererAPI
	{
		None = 0, OpenGL =1
	};

	enum class RendererDrawType
	{
		Triangles = 0, TriangleStrip = 1, TriangleFan = 3, Quads = 4
	};

	struct RendererInfo
	{
		std::string vendor;
		std::string version;
		std::string renderer;
	};

	class WYRD_LIBRARY_API Renderer
	{
	public:
		virtual ~Renderer() {}

		virtual void SetViewport(float width, float height) = 0;
		virtual void Clear(float r, float g, float b) const = 0;
		virtual void DrawElements(RendererDrawType type, uint32_t count) const = 0;
		virtual void DrawArray(RendererDrawType type, uint32_t offset, uint32_t count) const = 0;

		virtual void Initialise();
		virtual void Submit(DrawRectCommand& cmd);
		virtual void Submit(DrawSpriteCommand& cmd);
		virtual void Flush();

		inline struct RendererInfo& GetVendorInfo() { return _vendorInfo; }

		inline static RendererAPI GetAPI() { return s_RendererAPI; }

		static Renderer* Create();
	private:
		Vertex2DBatch _vertex2DBatch;
		SpriteBatch _spriteBatch;

	private:
		static RendererAPI s_RendererAPI;
		
	protected:
		struct RendererInfo _vendorInfo;
	};
}