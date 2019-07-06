#pragma once
#include "osrpch.h"

#include "SpriteLayer.h"

#include "core/Maths.h"

#include "SOIL.h"

namespace Osiris
{
	SpriteLayer::SpriteLayer()
	{
		/* Create Static batch table */
		_StaticSpriteBatch.reset(new SpriteBatch());

		/* TEMP: Load sprite batch textures */
		int width, height, channels;
		unsigned char* data;

		data = SOIL_load_image("../../Osiris/res/textures/test_01.bmp", &width, &height, &channels, SOIL_LOAD_AUTO);
		_StaticSpriteBatch->SetTexture(Texture::Create(data, width, height, channels));

		/* Load Test Sprites */
		AddSprite(new Sprite("Test Sprite", Maths::RandomFloat(100.0f, 500.0f), Maths::RandomFloat(100.0f, 500.0f), 50, 50));
		AddSprite(new Sprite("Test Sprite", Maths::RandomFloat(100.0f, 500.0f), Maths::RandomFloat(100.0f, 500.0f), 50, 50));
		AddSprite(new Sprite("Test Sprite", Maths::RandomFloat(100.0f, 500.0f), Maths::RandomFloat(100.0f, 500.0f), 50, 50));
		AddSprite(new Sprite("Test Sprite", Maths::RandomFloat(100.0f, 500.0f), Maths::RandomFloat(100.0f, 500.0f), 50, 50));
		AddSprite(new Sprite("Test Sprite", Maths::RandomFloat(100.0f, 500.0f), Maths::RandomFloat(100.0f, 500.0f), 50, 50));
	}

	void SpriteLayer::Render(Renderer& renderer)
	{
		_StaticSpriteBatch->Render(renderer);
	}

	void SpriteLayer::AddSprite(Sprite* sprite)
	{
		_Sprites.push_back(sprite);
		_StaticSpriteBatch->AddSprite(sprite);
	}
}