/* core wyrd includes */
#include <wyrdpch.h>
#include <core/Log.h>
#include <core/ecs/Components.h>
#include <core/Application.h>
#include <core/behaviour/ScriptedClass.h>

/* local includes */
#include "ScriptComponentView.h"
#include "support/ImGuiUtils.h"

/* external includes */
#include <imgui.h>

namespace Wyrd::Editor
{
	COMPONENT_VIEW_FACTORY_REGISTER(ScriptComponentView, "Script");

	void ScriptComponentView::OnEditorRender(Entity e, void* data)
	{
		/* Cast to the correct component */
		ScriptComponent* script = (ScriptComponent*)data;

		/* attempt to retrieve script class */
		std::shared_ptr<ScriptedClass> currentScriptClass = Application::Get().GetBehaviour().GetCustomClassByUID(script->scriptId);

		if (currentScriptClass != nullptr)
		{
			ImGui::Text("Script: %s", currentScriptClass->GetName().c_str());
		}
		else
		{
			ImGui::Text("Script: Unassigned");
		}

		if (ImGui::BeginDragDropTarget())
		{
			if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload(IMGUI_DND_SCRIPT))
			{
				UID* scriptUID = (UID*)payload->Data;

				/* retrieve script class */
				std::shared_ptr<ScriptedClass> scriptClass = Application::Get().GetBehaviour().GetCustomClassByUID(*scriptUID);

				if (scriptClass != nullptr)
				{
					/* assign the script */
					script->scriptId = *scriptUID;
					script->properties = scriptClass->GetPropertiesCopy();
				}

			}
			ImGui::EndDragDropTarget();
		}

		if (currentScriptClass != nullptr)
		{
			for (auto& prop : *script->properties)
			{
				// if this is empty this implied the script failure to parse the property, however the entity component is still expecting it.
				// this will clean up on the next save operation. See output for more details on which is prop is failing
				if (prop.second != nullptr)
				{
					if (ScriptPropertyViewFactory::Create(prop.second, nullptr) == false)
					{
						ImGui::TextColored({ 1.0f, 0.0f, 0.0f, 1.0f }, "Unabled to create view for property: %s", prop.first.c_str());
					}
				}
			}
		}
	}
}