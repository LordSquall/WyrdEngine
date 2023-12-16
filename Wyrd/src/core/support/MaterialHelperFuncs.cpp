#pragma once

/* core wyrd includes */
#include <wyrdpch.h>
#include <core/Application.h>
#include <core/ecs/Components.h>
#include <core/scene/Scene.h>
#include <core/support/MaterialHelperFuncs.h>


namespace Wyrd
{
	void MaterialHelperFuncs::AssignToComponent(MaterialComponent* materialComponent)
	{
		/* Retrieve the material */
		if (Application::Get().GetResources().Materials.find(materialComponent->material) != Application::Get().GetResources().Materials.end())
		{
			std::shared_ptr<Material> material = Application::Get().GetResources().Materials[materialComponent->material];

			MaterialInputMap propList = material->GetInputPropertyList();

			/* Clear out and recreate the property set */
			materialComponent->properties = std::make_shared<std::map<std::string, BasePropRef>>();

			/* Process each of the properties in the material and assign the data for the material component */
			for (auto& [name, binding] : propList)
			{
				(*materialComponent->properties)[name] = PropFactory::CreateProp(binding.type, name);
				(*materialComponent->properties)[name]->Deserialise(binding.defaultData);
			}
		}
		else
		{
			// default;
		}
	}
}