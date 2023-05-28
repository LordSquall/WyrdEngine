#pragma once

/* core wyrd includes */
#include <wyrdpch.h>
#include <core/Application.h>
#include <core/ecs/Components.h>
#include <core/scene/Scene.h>

/* local includes */
#include "MaterialHelperFuncs.h"

namespace Wyrd::Editor
{
	void MaterialHelperFuncs::AssignToComponent(MaterialComponent* materialComponent)
	{
		/* Retrieve the material */
		std::shared_ptr<Material> material = Application::Get().GetResources().Materials[materialComponent->material];

		Material::InputMap propList = material->GetInputPropertyList();

		/* Clear out and recreate the property set */
		materialComponent->properties = std::make_shared<std::map<std::string, BasePropRef>>();

		/* Process each of the properties in the material and assign the data for the material component */
		for (auto& [name, binding] : propList)
		{
			(*materialComponent->properties)[name] = PropFactory::CreateProp(binding.type, name);
		}
	}
}