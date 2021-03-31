#pragma once

/* core osiris includes */
#include <osrpch.h>
#include <core/export.h>
#include <core/scene/components/IBaseComponent.h>

/* local includes */
#include "views/DataModels/components/Transform2DComponentView.h"
#include "views/DataModels/components/SpriteComponentView.h"
#include "views/DataModels/components/PhysicsComponentView.h"
#include "views/DataModels/components/ScriptComponentView.h"
#include "views/DataModels/components/CameraComponentView.h"


namespace Osiris::Editor
{
	class GameObject;

	class PropertyViewFactory
	{
	public:
		static std::unique_ptr<IPropertiesView> Create(IBaseComponent* baseComponent, const Osiris::GameObject* gameObject)
		{
			std::unique_ptr<IPropertiesView> newComponent;

			switch (baseComponent->GetType())
			{
			case SceneComponentType::Transform2D:
				newComponent = std::make_unique<Transform2DComponentView>((Transform2DComponent*)baseComponent);
				break;
			case SceneComponentType::SpriteRenderer:
				newComponent = std::make_unique<SpriteComponentView>((SpriteComponent*)baseComponent);
				break;
			case SceneComponentType::PhysicsComponent:
				newComponent = std::make_unique<PhysicsComponentView>((PhysicsComponent*)baseComponent);
				break;
			case SceneComponentType::ScriptComponent:
				newComponent = std::make_unique<ScriptComponentView>((ScriptComponent*)baseComponent);
				break;
			case SceneComponentType::CameraComponent:
				newComponent = std::make_unique<CameraComponentView>((CameraComponent*)baseComponent);
				break;
			}

			return std::move(newComponent);
		}

	};
}