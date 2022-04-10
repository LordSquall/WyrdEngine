#pragma once

/* local includes */
#include "wyrdpch.h"
#include "core/export.h"
#include "events/Event.h"
#include "core/renderer/RendererDrawTypes.h"
#include "core/renderer/commands/RendererCommands.h"

namespace Wyrd
{
	enum class RendererAPI
	{
		None = 0, OpenGL =1
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
		virtual void Submit(DrawTextCommand& cmd);
		virtual void Submit(DrawVertex2DCommand& cmd);
		virtual void Flush();

		virtual void CopyPixels(int x, int y, int width, int height, unsigned char* buffer) = 0;

		inline struct RendererInfo& GetVendorInfo() { return _vendorInfo; }

		inline static RendererAPI GetAPI() { return s_RendererAPI; }

#ifdef WYRD_INCLUDE_DEBUG_TAGS
		virtual void StartNamedSection(const char* name) const;
		virtual void EndNamedSection() const;
#endif

		static Renderer* Create();
	private:
		//std::unique_ptr<Vertex2DBatch> _vertex2DBatch;
		//std::unique_ptr<SpriteBatch> _spriteBatch;
		//std::unique_ptr<TextBatch> _textBatch;

	private:
		static RendererAPI s_RendererAPI;
		
	protected:
		struct RendererInfo _vendorInfo;
	};
}