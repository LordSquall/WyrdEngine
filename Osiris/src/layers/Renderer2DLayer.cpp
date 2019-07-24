#pragma once
#include "osrpch.h"

#include "Renderer2DLayer.h"

namespace Osiris::Layers
{
	Renderer2DLayer::Renderer2DLayer(std::string name) : Layer(name)
	{
		/* Setup the Ortho Matrix */
		float right = (float)Application::Get().GetWindow().GetWidth();
		float left = 0.0f;
		float top = (float)Application::Get().GetWindow().GetHeight();
		float bottom = 0.0f;

		_vpMat = glm::ortho(left, right, bottom, top);
	}

	void Renderer2DLayer::OnAttach()
	{
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

		/* Add initial sprite layer */
		SpriteLayer* spriteLayer = new SpriteLayer();
		spriteLayer->SetName("Background");

		_SpriteLayers.push_back(spriteLayer);
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

		/* Render Each sprite layer */
		for (auto sl : _SpriteLayers)
		{
			sl->Render(renderer);
		}
	}


	SpriteLayer* Renderer2DLayer::AddSpriteLayer(std::string name)
	{
		SpriteLayer* newLayer = new SpriteLayer();
		newLayer->SetName(name);
		_SpriteLayers.push_back(newLayer);
		return newLayer;
	}

	void Renderer2DLayer::RemoveSpriteLayer(uint32_t index)
	{
		_SpriteLayers.erase(_SpriteLayers.begin() + index);
	}
	
	void Renderer2DLayer::RemoveAllSpriteLayers()
	{
		_SpriteLayers.clear();
	}
}