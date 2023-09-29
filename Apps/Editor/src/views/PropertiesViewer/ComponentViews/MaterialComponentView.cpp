/* core wyrd includes */
#include <wyrdpch.h>
#include <core/Log.h>
#include <core/ecs/Components.h>
#include <core/Application.h>
#include <core/support/MaterialHelperFuncs.h>

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

		/* Retrieve the material */
		std::shared_ptr<Material> material = Application::Get().GetResources().Materials[materialComponent->material];
		
		ImGui::Text(material->GetName().c_str());
		if (ImGui::BeginDragDropTarget())
		{
			if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload(IMGUI_DND_MATERIAL))
			{
				UID* materialUID = (UID*)payload->Data;
				materialComponent->material = *materialUID;

				MaterialHelperFuncs::AssignToComponent(materialComponent);
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