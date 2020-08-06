#pragma once

/* core osiris includes */
#include <osrpch.h>
#include <core/export.h>
#include <core/scene/components/IBaseComponent.h>

/* local includes */
#include "views/DataModels/components/SpriteComponentView.h"
#include "views/DataModels/components/PhysicsComponentView.h"
#include "views/DataModels/components/ScriptComponentView.h"


namespace Osiris::Editor
{
	class GameObject;

	class PropertyViewFactory
	{
	public:
		static std::shared_ptr<IPropertiesView> Create(std::shared_ptr<IBaseComponent> baseComponent, const Osiris::GameObject* gameObject)
		{
			std::shared_ptr<IPropertiesView> newComponent;

			switch (baseComponent->GetType())
			{
			case SceneComponentType::SpriteRenderer:
				newComponent = std::make_shared<SpriteComponentView>((SpriteComponent*)&*baseComponent);
				break;
			case SceneComponentType::PhysicsComponent:
				newComponent = std::make_shared<PhysicsComponentView>((PhysicsComponent*)&*baseComponent);
				break;
			case SceneComponentType::ScriptComponent:
				newComponent = std::make_shared<ScriptComponentView>((ScriptComponent*)&*baseComponent);
				break;
			}

			return newComponent;
		}

	};
}