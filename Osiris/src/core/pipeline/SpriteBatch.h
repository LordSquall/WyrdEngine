#pragma once

#include <Osiris.h>

#include "Sprite.h"

using namespace Osiris;


namespace Osiris {

	/* represents a single sprite vertex */
	struct SpriteVertex
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

		void AddSprite(Sprite* sprite);

		void SetTexture(Texture* texture);

	private:

		std::vector<SpriteVertex> _vertices;
		std::vector<unsigned int> _indicies;
		std::unique_ptr<VertexArray> _VertexArray;
		std::unique_ptr<VertexBuffer> _VertexBuffer;
		std::unique_ptr<IndexBuffer> _IndexBuffer;

		std::map<uint32_t, SpriteBatchEntry> _SpriteMap;

		std::unique_ptr<Texture> _Texture;
	};

	/* represents a single sprite entry in a batch table */
	struct SpriteBatchEntry
	{
		/* sprite source data model */
		Sprite* sprite;

		/* position within the parent batch data */
		uint32_t offset;

		/* Update the batch data based on the source data model */
		void Update(SpriteBatch* batch);
	};
}