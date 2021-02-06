#pragma once

/* local includes */
#include "osrpch.h"
#include "SpriteComponent.h"
#include "core/Application.h"
#include "core/Resources.h"
#include "core/renderer/Texture.h"
#include "core/renderer/Shader.h"
#include "core/renderer/VertexArray.h"
#include "core/renderer/Buffer.h"
#include "core/pipeline/SpriteVertex.h"
#include "core/scene/GameObject.h"
#include "core/scene/components/Transform2DComponent.h"

namespace Osiris
{
	SpriteComponent::SpriteComponent(std::shared_ptr<GameObject> owner) : IBaseComponent(owner, SceneComponentType::SpriteRenderer)
		, position(glm::vec2(0.0f, 0.0f)), size(glm::vec2(64.0f, 64.0f)), color({ 1.0f, 1.0f, 1.0f, 1.0f })
	{

	}

	SpriteComponent::SpriteComponent(const SpriteComponent& obj) : IBaseComponent(obj.Owner, SceneComponentType::SpriteRenderer)
	{
		shader = obj.shader;
		texture = obj.texture;
		position = obj.position;
		size = obj.size;
		color = obj.color;
	}

	void SpriteComponent::Initialise()
	{
		
		Initialised = true;


		Recalculate();
	}

	void SpriteComponent::Recalculate()
	{
		Owner->inputArea = { 0.0, 0.0, size.x, size.y};
	}

	void SpriteComponent::Render(Timestep ts, Renderer& renderer)
	{

	}
}