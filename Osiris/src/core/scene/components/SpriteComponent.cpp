#pragma once

/* local includes */
#include "osrpch.h"
#include "SpriteComponent.h"
#include "core/Application.h"
#include "core/Resources.h"
#include "core/Log.h"
#include "core/renderer/Texture.h"
#include "core/renderer/Shader.h"
#include "core/renderer/VertexArray.h"
#include "core/renderer/Buffer.h"
#include "core/pipeline/SpriteVertex.h"
#include "core/pipeline/SpriteBatch.h"
#include "core/scene/SceneLayer2D.h"
#include "core/scene/GameObject.h"
#include "core/scene/components/Transform2DComponent.h"
#include "serial/TypeSerialisers.h"

#include <jsonxx.h>

namespace Osiris
{
	SpriteComponent::SpriteComponent(GameObject* gameObject) : IBaseComponent(gameObject, SceneComponentType::SpriteRenderer)
		, position(glm::vec2(0.0f, 0.0f)), size(glm::vec2(64.0f, 64.0f)), color({ 1.0f, 1.0f, 1.0f, 1.0f }), BatchIndex(0)
	{
		_Texture = Application::Get().GetResources().Textures[UID(RESOURCE_DEFAULT_TEXTURE)];
	}

	bool SpriteComponent::Initialise()
	{
		SceneLayer2D* layer2D = dynamic_cast<SceneLayer2D*>(Owner->GetSceneLayer());
		if (layer2D != nullptr)
		{
			sceneLayer = layer2D;
			BatchIndex = layer2D->GetSpriteBatch()->AddSprite(this);
		}
		else
		{
			OSR_TRACE("Unable to add sprite component to the game object on a none 2D scene layer");
			return false;
		}

		return true;
	}

	void SpriteComponent::SetSize(float width, float height)
	{
		size.x = width;
		size.y = height;

		Owner->inputArea = { position.x, position.y, size.x, size.y };

		sceneLayer->GetSpriteBatch()->UpdateSprite(this);
	}

	void SpriteComponent::SetTexture(std::shared_ptr<Texture> texture)
	{
		_Texture = texture;

		SetUID(texture->GetUID());
	}

	const std::shared_ptr<Texture> SpriteComponent::GetTexture()
	{
		return _Texture;
	}

	void SpriteComponent::SetPosition(float x, float y)
	{
		position.x = x;
		position.y = y;

		Owner->inputArea = { position.x, position.y, size.x, size.y };

		sceneLayer->GetSpriteBatch()->UpdateSprite(this);
	}


	void SpriteComponent::Recalculate()
	{

	}

	void SpriteComponent::Render(Timestep ts, Renderer& renderer)
	{

	}

	void SpriteComponent::Remove()
	{
		//spriteLayer->GetSpriteBatch().RemoveSprite(this);
	}

	bool SpriteComponent::ToJson(jsonxx::Object& object)
	{
		object << "componentType" << (int)SceneComponentType::SpriteRenderer;

		/* texture */
		object << "texture" << _Texture->GetUID().str();

		/* size */
		object << "size" << size;

		/* position offset */
		object << "position" << position;

		/* blend color */
		object << "blendColor" << color;

		return true;
	}

	bool SpriteComponent::FromJson(jsonxx::Object& object)
	{
		Initialise();

		/* texture */
		_Texture = Application::Get().GetResources().Textures[UID(object.get<jsonxx::String>("texture", RESOURCE_DEFAULT_TEXTURE))];

		/* size */
		if (object.has<jsonxx::Array>("size"))
			size << object.get<jsonxx::Array>("size");

		/* position offset */
		if (object.has<jsonxx::Array>("position"))
			position << object.get<jsonxx::Array>("position");

		/* blend color */
		if (object.has<jsonxx::Array>("blendColor"))
			color << object.get<jsonxx::Array>("blendColor");

		return true;
	}
}