#pragma once

#include "core/renderer/Renderer.h"

using namespace Osiris;

namespace Osiris {

	class Texture;
	class VertexArray;
	class VertexBuffer;
	class IndexBuffer;
	class Shader;
	class SpriteComponent;

	/* represents a single sprite vertex */
	struct SpriteVertex2
	{
		float x, y;		/* 2D position		*/
		float u, v;		/* texture coords	*/
	};

	/* respresents a batch a sprint geometry */
	class SpriteBatch
	{
		/* allow access to internal vertices and index buffers */
		friend struct SpriteBatchEntry;

	public:
		SpriteBatch();
		~SpriteBatch() {}

		void Render(Renderer& renderer, const glm::mat4& viewProjectionMat);

		void AddSprite(std::shared_ptr<SpriteComponent> sprite);

		void RemoveSprite(SpriteComponent* sprite);

		void SetShader(std::shared_ptr<Shader> shader);

	private:

		std::vector<SpriteVertex2> _vertices;
		std::shared_ptr<VertexArray> _VertexArray;
		std::shared_ptr<VertexBuffer> _VertexBuffer;
		std::shared_ptr<Shader> _Shader;

		std::vector<SpriteBatchEntry> _SpriteMap;
	};

	/* represents a single sprite entry in a batch table */
	struct SpriteBatchEntry
	{
		/* sprite component source data model */
		std::shared_ptr<SpriteComponent> sprite;

		/* position within the parent batch data */
		uint32_t offset;

		/* current parent batch */
		SpriteBatch* batch;
	};
}