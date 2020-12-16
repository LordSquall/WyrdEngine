#pragma once

#include "core/renderer/Renderer.h"
#include "core/pipeline/Sprite.h"

using namespace Osiris;


namespace Osiris {

	class Texture;
	class VertexArray;
	class VertexBuffer;
	class IndexBuffer;

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

		void Render(Renderer& renderer);

		void AddSprite(std::shared_ptr<Sprite> sprite);

		void SetTexture(Texture* texture);

	private:

		std::vector<SpriteVertex2> _vertices;
		std::vector<unsigned int> _indicies;
		std::shared_ptr<VertexArray> _VertexArray;
		std::shared_ptr<VertexBuffer> _VertexBuffer;
		std::shared_ptr<IndexBuffer> _IndexBuffer;

		std::map<uint32_t, SpriteBatchEntry> _SpriteMap;

		std::shared_ptr<Texture> _Texture;
	};

	/* represents a single sprite entry in a batch table */
	struct SpriteBatchEntry
	{
		/* sprite source data model */
		std::shared_ptr<Sprite> sprite;

		/* position within the parent batch data */
		uint32_t offset;

		/* current parent batch */
		SpriteBatch* batch;

		/* Update the batch data based on the source data model */
		void Update();
	};
}