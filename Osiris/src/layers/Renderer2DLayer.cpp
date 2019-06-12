#pragma once
#include "osrpch.h"

#include "Renderer2DLayer.h"

#include "SOIL.h"

namespace Osiris::Layers
{
	Renderer2DLayer::Renderer2DLayer(std::string name) : Layer(name)
	{
		/* Setup the Ortho Matrix */
		float right = Application::Get().GetWindow().GetWidth();
		float left = 0.0f;
		float top = Application::Get().GetWindow().GetHeight();
		float bottom = 0.0f;

		_vpMat = glm::ortho(left, right, bottom, top);
	}

	void Renderer2DLayer::OnAttach()
	{
		/* Create Static batch table */
		_staticSpriteBatch.reset(new SpriteBatch());

		/* Load any resources which are required to renderer (e.g. shaders) */
		std::ifstream vertexStream("../../Osiris/res/shaders/sprite.vs");
		std::string vertexSrc((std::istreambuf_iterator<char>(vertexStream)), std::istreambuf_iterator<char>());

		std::ifstream fragmentStream("../../Osiris/res/shaders/sprite.fs");
		std::string fragmentSrc((std::istreambuf_iterator<char>(fragmentStream)), std::istreambuf_iterator<char>());

		_Shader.reset(Shader::Create());
		_Shader->Build(vertexSrc, fragmentSrc);
		_Shader->Bind();

		/* Set the View * Projection Matrix */
		_Shader->SetVPMatrix(_vpMat);

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

	void Renderer2DLayer::OnDetach()
	{
		/* Close down Static Batching table */

		/* Release core resources */
	}

	void Renderer2DLayer::OnUpdate()
	{
		/* Perform any registers 2D Update functions */
	}

	void Renderer2DLayer::OnEvent(Event& event)
	{
		/* User Events ??*/
	}

	void Renderer2DLayer::OnRender(Renderer& renderer)
	{
		/* Bind the sprite shader */
		_Shader->Bind();

		/* Render Static Batching Table */
		_staticSpriteBatch->Render(renderer);
	}
}