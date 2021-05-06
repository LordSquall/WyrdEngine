#pragma once

/* local includes  */
#include <core/export.h>
#include <core/Timestep.h>
#include <core/systems/ISystem.h>

#include "extensions/systems/EditorComponent.h"

namespace Wyrd::Editor {

	class WYRD_LIBRARY_API EditorSystem : public ISystem
	{
	public:
		virtual bool OnUpdate() override;

		EditorComponent* CreateComponent(GameObject* gameObject);

		GameObject* FindInWorldSpace(glm::vec2 worldPosition);

	private:
		std::vector<std::unique_ptr<EditorComponent>> _Components;
	};
}