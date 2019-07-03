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

		void AddSprite(Sprite* sprite);

		inline std::vector<Sprite*> GetSprites() const { return _Sprites; }

		inline void SetName(std::string name) { _Name = name; }
		inline std::string GetName() const { return _Name; }

	private:
		std::string _Name;
		std::vector<Sprite*> _Sprites;
		std::unique_ptr<SpriteBatch> _StaticSpriteBatch;

	};
}