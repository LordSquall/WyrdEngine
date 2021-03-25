#pragma once
#include "osrpch.h"

/* Core includes */
#include "core/renderer/Renderer.h"
#include "core/renderer/Shader.h"
#include "core/pipeline/SpriteBatch.h"
#include "core/scene/components/SpriteComponent.h"
#include "core/Resources.h"

/* Local includes */
#include "Layer2D.h"

namespace Osiris
{
	Layer2D::Layer2D() : GameObject("Untitled") { }

	Layer2D::Layer2D(const std::string& name) : GameObject(name) { }

	Layer2D::Layer2D(const Layer2D& obj) : GameObject(obj.name) { }

	Layer2D::~Layer2D() { }


	bool Layer2D::Initialise()
	{
		/* Initiailse the sprite batch */
		_SpriteBatch = std::make_shared<SpriteBatch>();

		_SpriteBatch->SetShader(Resources::Get().Shaders["Sprite"]);

		return true;
	}

	void Layer2D::Update() { }

	void Layer2D::Render(Renderer& renderer, const glm::mat4& viewProjectionMat)
	{
		_SpriteBatch->Render(renderer, viewProjectionMat);
	}

	bool Layer2D::RegisterSprite(std::shared_ptr<SpriteComponent> spriteComponent)
	{
		//spriteComponent->BatchIndex = _SpriteBatch->AddSprite(spriteComponent);

		return true;
	}


	void Layer2D::AssignScripts(Behaviour* behaviour)
	{
		for (auto& go : _GameObjects)
		{
			go->AssignScripts(behaviour);
		}
	}
}