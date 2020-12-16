#pragma once

#include "core/renderer/Shader.h"
#include "core/pipeline/SpriteBatch.h"

using namespace Osiris;

namespace Osiris {

	class SpriteLayer
	{
	public:
		SpriteLayer();

		~SpriteLayer() {}

		void Render(Renderer& renderer);

		void AddSprite(std::shared_ptr<Sprite> sprite);

		void RemoveSprite(int spriteIdx);

		void SwapSprites(int spriteIdxSrc, int spriteIdxDst);

		inline std::vector<std::shared_ptr<Sprite>> GetSprites() const { return _Sprites; }

		inline void SetName(std::string name) { _Name = name; }
		inline std::string GetName() const { return _Name; }

		inline void SetShader(std::shared_ptr<Shader> shader) { _Shader = shader; }
		inline std::shared_ptr<Shader> GetShader() const { return _Shader; }

	private:
		std::string _Name;
		std::vector<std::shared_ptr<Sprite>> _Sprites;
		std::shared_ptr<Shader> _Shader;
	};
}