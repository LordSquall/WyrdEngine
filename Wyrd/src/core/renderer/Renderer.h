#pragma once

/* local includes */
#include "wyrdpch.h"
#include "core/export.h"
#include "core/maths/BoundingBox.h"
#include "core/maths/Rect.h"
#include "core/maths/Frustum.h"
#include "events/Event.h"
#include "core/renderer/Buffer.h"
#include "core/renderer/VertexArray.h"
#include "core/renderer/commands/RendererCommands.h"
#include "core/renderer/RendererDrawTypes.h"
#include "core/pipeline/MeshBatch.h"
#include "core/pipeline/Vertex2DBatch.h"
#include "core/pipeline/Vertex3DBatch.h"
#include "core/pipeline/SpriteBatch.h"
#include "core/pipeline/TextBatch.h"

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
		virtual void Submit(DrawMeshCommand& cmd);
		virtual void Submit(DrawSpriteCommand& cmd);
		virtual void Submit(DrawTextCommand& cmd);
		virtual void Submit(DrawVertex2DCommand& cmd);
		virtual void Submit(DrawVertex3DCommand& cmd);
		virtual void Submit(DrawMeshInputColorPickerCommand& cmd);
		virtual void Flush();

		virtual void DrawDebugBoundingBox(const BoundingBox& boundingBox, const glm::vec3& position, float thickness, const Color& color, const glm::mat4& model, const glm::mat4& projection, const glm::mat4& view) = 0;
		virtual void DrawDebugVector(const Vector3& position, const glm::vec3& direction, float thickness, const Color& color, const glm::mat4& model, const glm::mat4& projection, const glm::mat4& view) = 0;
		virtual void DrawDebugFrustum(const Vector3& position, const glm::vec3& direction, const Frustum& frustum, float thickness, const Color& color, const glm::mat4& model, const glm::mat4& projection, const glm::mat4& view) = 0;
		
		virtual void SetFallbackMaterial(std::shared_ptr<Material> fallbackMaterial);

		inline struct RendererInfo& GetVendorInfo() { return _vendorInfo; }

		inline static RendererAPI GetAPI() { return s_RendererAPI; }

#ifdef WYRD_INCLUDE_DEBUG_TAGS
		virtual void StartNamedSection(const char* name) const;
		virtual void EndNamedSection() const;
#endif

		static Renderer* Create();
	protected:
		MeshBatch _MeshBatch;
		Vertex2DBatch _vertex2DBatch;
		Vertex3DBatch _vertex3DBatch;
		SpriteBatch _spriteBatch;
		TextBatch _textBatch;

		std::vector<Vertex3D>			_DebugVertices;
		std::shared_ptr<VertexArray>	_DebugVertexArray;
		std::shared_ptr<VertexBuffer>	_DebugVertexBuffer;

	private:
		static RendererAPI s_RendererAPI;
		
	protected:
		struct RendererInfo _vendorInfo;
	};
}