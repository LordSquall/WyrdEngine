#pragma once

/* core wyrd includes */
#include <wyrdpch.h>
#include <core/Log.h>
#include <core/scene/components/TransformComponent.h>
#include <core/scene/components/SpriteComponent.h>

#include "extensions/systems/EditorSystem.h"

namespace Wyrd::Editor
{

	bool EditorSystem::OnUpdate()
	{
		for (auto& c : _Components)
		{
			Rect inputArea = { { 0.0f, 0.0f}, { 0.0f, 0.0f } };

			for (auto& comp : c->Owner->components)
			{
				SpriteComponent* spriteComponent = dynamic_cast<SpriteComponent*>(comp.get());
				if (spriteComponent != nullptr)
				{
					inputArea.position = spriteComponent->position;
					inputArea.size = spriteComponent->size;
				}
			}

			glm::vec4 localPos = { inputArea.position.x, inputArea.position.y, 0.0f, 1.0f};
			glm::vec4 worldPos = c->Owner->transform->GetModelMatrix() * localPos;

			c->inputArea.position.x = worldPos.x; 
			c->inputArea.position.y = worldPos.y;
			c->inputArea.size = inputArea.size;
		}
		return true;
	}


	EditorComponent* EditorSystem::CreateComponent(GameObject* gameObject)
	{
		_Components.push_back(std::make_unique<EditorComponent>(gameObject));

		return _Components.back().get();
	}


	GameObject* EditorSystem::FindInWorldSpace(glm::vec2 worldPosition)
	{
		for (std::vector<std::unique_ptr<EditorComponent>>::reverse_iterator comp = _Components.rbegin(); comp != _Components.rend(); ++comp)
		{
			if ((*comp)->inputArea.ContainsPoint(worldPosition) == true)
			{
				return (*comp)->Owner;
			}
		}

		return nullptr;
	}
}