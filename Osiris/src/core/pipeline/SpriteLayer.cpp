#pragma once
#include "osrpch.h"

#include "SpriteLayer.h"

#include "core/renderer/VertexArray.h"
#include "core/renderer/Buffer.h"
#include "core/renderer/Texture.h"
#include "core/pipeline/SpriteVertex.h"
#include "core/pipeline/Sprite.h"

#include "core/Maths.h"

#include "SOIL.h"

namespace Osiris
{
	SpriteLayer::SpriteLayer()
	{
		/* Create Static batch table */
		//_StaticSpriteBatch.reset(new SpriteBatch());
	}

	void SpriteLayer::Render(Renderer& renderer)
	{
		for (auto sprite : _Sprites)
		{
			sprite->GetVertexArray()->Bind();
			sprite->GetIndexBuffer()->Bind();
			sprite->GetVertexBuffer()->Bind();


			if (sprite->GetTexture() != nullptr)
				sprite->GetTexture()->Bind();


			renderer.DrawElements(RendererDrawType::Triangles, sprite->GetIndexBuffer()->GetCount());
		}
	}

	void SpriteLayer::AddSprite(std::shared_ptr<Sprite> sprite)
	{
		_Sprites.push_back(sprite);
	}


	void SpriteLayer::RemoveSprite(int spriteIdx)
	{
		_Sprites.erase(_Sprites.begin() + spriteIdx);
	}


	void SpriteLayer::SwapSprites(int spriteIdxSrc, int spriteIdxDst)
	{
		std::shared_ptr<Sprite> tmp = _Sprites[spriteIdxSrc];
		_Sprites[spriteIdxSrc] = _Sprites[spriteIdxDst];
		_Sprites[spriteIdxDst] = tmp;
	}
}