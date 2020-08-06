#pragma once

/* local includes */
#include "osrpch.h"
#include "SpriteComponent.h"
#include "core/scene/GameObject.h"
#include "core/scene/components/Transform2DComponent.h"

namespace Osiris
{
	SpriteComponent::SpriteComponent(std::shared_ptr<GameObject> owner) : IBaseComponent(owner, SceneComponentType::SpriteRenderer)
		, Color({ 1.0f, 1.0f, 1.0f, 1.0f })
	{
		Shader = Resources::Get().Shaders["Sprite"];
		Sprite = std::make_shared<Osiris::Sprite>("NewSprite", 0.0f, 0.0f, 64.0f, 64.0f);
		BaseTexture = Resources::Get().Textures["DefaultSprite"];
		Color = { 1.0f, 1.0f, 1.0f, 1.0f };
	}

	SpriteComponent::SpriteComponent(const SpriteComponent& obj) : IBaseComponent(obj.Owner, SceneComponentType::SpriteRenderer)
	{
		Shader = obj.Shader;
		BaseTexture = obj.BaseTexture;
		Sprite = obj.Sprite;
		Color = obj.Color;
	}

	SpriteComponent::~SpriteComponent()
	{

	}

	void SpriteComponent::Recalculate()
	{
		Owner->inputArea = { 0.0, 0.0, Sprite->GetWidth(), Sprite->GetHeight()};
	}

	void SpriteComponent::Render(Timestep ts, Renderer& renderer)
	{
		Sprite->GetVertexArray()->Bind();
		Sprite->GetVertexBuffer()->Bind();
		Sprite->GetIndexBuffer()->Bind();
		BaseTexture->Bind();

		Shader->SetModelMatrix(Owner->transform2D->matrix);
		Shader->SetUniformVec4("blendColor", Color);

		renderer.DrawElements(RendererDrawType::Triangles, 6);
	}
}