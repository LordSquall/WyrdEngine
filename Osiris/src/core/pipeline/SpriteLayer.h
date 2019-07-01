#pragma once

#include <Osiris.h>

#include "core/pipeline/SpriteBatch.h"

using namespace Osiris;

namespace Osiris {

	class SpriteLayer
	{
	public:
		SpriteLayer();

		~SpriteLayer() {}

		void Render(Renderer& renderer);

	private:
		std::unique_ptr<SpriteBatch> _staticSpriteBatch;
	};
}