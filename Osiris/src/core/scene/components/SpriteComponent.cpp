#pragma once

/* local includes */
#include "osrpch.h"
#include "Osiris.h"
#include "SpriteComponent.h"
#include "core/renderer/Texture.h"
#include "core/renderer/Shader.h"
#include "core/pipeline/Sprite.h"
#include "core/scene/GameObject.h"
#include "core/scene/components/Transform2DComponent.h"

namespace Osiris
{
	SpriteComponent::SpriteComponent(std::shared_ptr<GameObject> owner) : IBaseComponent(owner, SceneComponentType::SpriteRenderer)
		, Color({ 1.0f, 1.0f, 1.0f, 1.0f })
	{
		shader = Resources::Get().Shaders["Sprite"];
		sprite = std::make_shared<Osiris::Sprite>("NewSprite", 0.0f, 0.0f, 64.0f, 64.0f);
		BaseTexture = Resources::Get().Textures["DefaultSprite"];
		Color = { 1.0f, 1.0f, 1.0f, 1.0f };
	}

	SpriteComponent::SpriteComponent(const SpriteComponent& obj) : IBaseComponent(obj.Owner, SceneComponentType::SpriteRenderer)
	{
		shader = obj.shader;
		BaseTexture = obj.BaseTexture;
		sprite = obj.sprite;
		Color = obj.Color;
	}

	SpriteComponent::~SpriteComponent()
	{

	}


	void SpriteComponent::Initialise()
	{
		Recalculate();
	}

	void SpriteComponent::Recalculate()
	{
		Owner->inputArea = { 0.0, 0.0, sprite->GetWidth(), sprite->GetHeight()};
	}

	void SpriteComponent::Render(Timestep ts, Renderer& renderer)
	{
		sprite->GetVertexArray()->Bind();
		sprite->GetVertexBuffer()->Bind();
		sprite->GetIndexBuffer()->Bind();
		BaseTexture->Bind();

		shader->SetModelMatrix(Owner->transform2D->matrix);
		shader->SetUniformVec4("blendColor", Color);

		renderer.DrawElements(RendererDrawType::Triangles, 6);
	}
}