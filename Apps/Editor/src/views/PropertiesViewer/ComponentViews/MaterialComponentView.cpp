/* core wyrd includes */
#include <wyrdpch.h>
#include <core/Log.h>
#include <core/ecs/Components.h>
#include <core/Application.h>

/* local includes */
#include "MaterialComponentView.h"
#include "views/PropertiesViewer/PropGUIViews/PropGUIFactory.h"

/* external includes */
#include <imgui.h>

namespace Wyrd::Editor
{
	COMPONENT_VIEW_FACTORY_REGISTER(MaterialComponentView, "Material");

	void MaterialComponentView::OnEditorRender(Entity e, void* data)
	{
		/* Cast to the correct component */
		MaterialComponent* materialComponent = (MaterialComponent*)data;
		
		ImGui::Text(materialComponent->material.str().c_str());
		if (ImGui::BeginDragDropTarget())
		{
			if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload(IMGUI_DND_MATERIAL))
			{
				UID* materialUID = (UID*)payload->Data;
				materialComponent->material = *materialUID;

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
			ImGui::EndDragDropTarget();
		}

		if (materialComponent->properties != nullptr)
		{
			for (auto& inputProp : *materialComponent->properties)
			{
				if (inputProp.second)
				{
					PropGUIFactory::DrawProp(inputProp.second.get());
				}
				else 
				{
					PropGUIFactory::DrawMissingProp(inputProp.first);
				}
			}
		}
	}
}