#pragma once

/* core osiris includes */
#include <osrpch.h>
#include <core/export.h>
#include <core/scene/components/IBaseComponent.h>
#include <core/scene/components/Transform2DComponent.h>
#include <core/scene/components/Transform3DComponent.h>
#include <core/scene/components/SpriteComponent.h>
#include <core/scene/components/ScriptComponent.h>
#include <core/scene/components/PhysicsComponent.h>
#include <core/scene/components/CameraComponent.h>


namespace Osiris
{
	class GameObject;

	class SceneComponentFactory
	{
	public:
		static std::unique_ptr<IBaseComponent> Create(SceneComponentType componentType, Osiris::GameObject* gameObject)
		{
			std::unique_ptr<IBaseComponent> newComponent;

			switch (componentType)
			{
			case SceneComponentType::Transform2D:
				newComponent = std::make_unique<Transform2DComponent>(gameObject);
				break;
			case SceneComponentType::SpriteRenderer:
				newComponent = std::make_unique<SpriteComponent>(gameObject);
				break;
			case SceneComponentType::PhysicsComponent:
				newComponent = std::make_unique<PhysicsComponent>(gameObject);
				break;
			case SceneComponentType::ScriptComponent:
				newComponent = std::make_unique<ScriptComponent>(gameObject);
				break;
			case SceneComponentType::CameraComponent:
				newComponent = std::make_unique<CameraComponent>(gameObject);
				break;
			}

			return std::move(newComponent);
		}

	};
}