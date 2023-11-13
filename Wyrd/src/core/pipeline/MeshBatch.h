#pragma once

#include "core/renderer/commands/RendererCommands.h"
#include "core/renderer/RendererDrawTypes.h"
#include "core/renderer/Material.h"
#include "core/pipeline/Vertex3D.h"

using namespace Wyrd;

namespace Wyrd {

	class Renderer;
	class Texture;
	class VertexArray;
	class VertexBuffer;
	class IndexBuffer;
	class Shader;

	/* respresents a batch a model geometry */
	class MeshBatch
	{
	public:
		~MeshBatch() = default;

		bool Initialise(Renderer* renderer);

		void Submit(DrawMeshCommand& cmd);

		void Flush();

		inline void SetFallbackMaterial(std::shared_ptr<Material> material) { _FallbackMaterial = material; }

	private:
		Renderer*						_Renderer;
		std::shared_ptr<VertexArray>	_VertexArray;
		std::shared_ptr<VertexBuffer>	_VertexBuffer;
		INT32							_VertexCount;

		glm::mat4						_ModelMatrix;
		glm::mat4						_ViewMatrix;
		glm::mat4						_ProjectionMatrix;
		Material*						_Material;
		Color							_Color;
		Texture*						_BaseTexture;
		BasePropMapRef*					_MaterialProps;
		RendererDrawType				_DrawType;

		std::shared_ptr<Material>		_FallbackMaterial;
	};
}