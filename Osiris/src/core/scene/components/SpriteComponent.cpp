#pragma once

/* local includes */
#include "osrpch.h"
#include "SpriteComponent.h"
#include "core/Resources.h"
#include "core/renderer/Texture.h"
#include "core/renderer/Shader.h"
#include "core/renderer/VertexArray.h"
#include "core/renderer/Buffer.h"
#include "core/pipeline/SpriteVertex.h"
#include "core/pipeline/Sprite.h"
#include "core/scene/GameObject.h"
#include "core/scene/components/Transform2DComponent.h"

namespace Osiris
{
	SpriteComponent::SpriteComponent(std::shared_ptr<GameObject> owner) : IBaseComponent(owner, SceneComponentType::SpriteRenderer)
		, color({ 1.0f, 1.0f, 1.0f, 1.0f })
	{
		shader = Resources::Get().Shaders["Sprite"];
		sprite = std::make_shared<Osiris::Sprite>("NewSprite", 0, 0, 64, 64);
		BaseTexture = Resources::Get().Textures["DefaultSprite"];
		color = { 1.0f, 1.0f, 1.0f, 1.0f };
	}

	SpriteComponent::SpriteComponent(const SpriteComponent& obj) : IBaseComponent(obj.Owner, SceneComponentType::SpriteRenderer)
	{
		shader = obj.shader;
		BaseTexture = obj.BaseTexture;
		sprite = obj.sprite;
		color = obj.color;
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
		shader->SetUniformVec4("blendColor", glm::vec4(color.r, color.g, color.b, color.a));

		renderer.DrawElements(RendererDrawType::Triangles, 6);
	}
}