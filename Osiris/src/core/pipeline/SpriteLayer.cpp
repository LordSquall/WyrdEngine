#pragma once
#include "osrpch.h"

#include "SpriteLayer.h"

#include "SOIL.h"

namespace Osiris
{
	SpriteLayer::SpriteLayer()
	{
		/* Create Static batch table */
		_staticSpriteBatch.reset(new SpriteBatch());

		/* TEMP: Load sprite batch textures */
		int width, height, channels;
		unsigned char* data;

		data = SOIL_load_image("../../Osiris/res/textures/test_01.bmp", &width, &height, &channels, SOIL_LOAD_AUTO);
		_staticSpriteBatch->SetTexture(Texture::Create(data, width, height, channels));

		/* TEMP: Load Test Sprites */
		_staticSpriteBatch->AddSprite(new Sprite(200, 200, 100, 100));
		/*_staticSpriteBatch->AddSprite(new Sprite(100, 100, 100, 100));
		_staticSpriteBatch->AddSprite(new Sprite(200, 200, 50, 50));
		_staticSpriteBatch->AddSprite(new Sprite(300, 300, 100, 100));
		_staticSpriteBatch->AddSprite(new Sprite(400, 400, 50, 50));*/
	}

	void SpriteLayer::Render(Renderer& renderer)
	{
		_staticSpriteBatch->Render(renderer);
	}
}