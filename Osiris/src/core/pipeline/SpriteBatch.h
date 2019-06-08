#pragma once

#include <Osiris.h>

#include "Sprite.h"

using namespace Osiris;


namespace Osiris {

	class SpriteBatch
	{
	public:
		SpriteBatch();

		~SpriteBatch() {}

		void Render(Renderer& renderer);

		void AddSprite(Sprite* sprite);

	private:

		std::vector<float> _vertices;
		std::vector<unsigned int> _indicies;
		std::unique_ptr<VertexArray> _VertexArray;
		std::unique_ptr<VertexBuffer> _VertexBuffer;
		std::unique_ptr<IndexBuffer> _IndexBuffer;
	};
}