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

		/* retrieve script class */
		std::shared_ptr<ScriptedClass> currentScriptClass = Application::Get().GetBehaviour().GetCustomClassByUID(script->script);

		if (currentScriptClass != nullptr)
		{
			ImGui::Text("Script: %s", currentScriptClass->GetName().c_str());
		}
		else
		{
			ImGui::Text("Script: %s", script->script.str().c_str());
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
					script->script = *scriptUID;

					/* assign property count */
					script->propertyCount = scriptClass->Properties.size();

					/* clear the property map */
					script->propertyMap.clear();

					/* remap the values to match the slots for the new script */
					int i = 0;
					for (auto& prop : scriptClass->Properties)
					{
						/* reset the internal buffer */
						memset(script->properties[i], 0, sizeof(char) * SCRIPT_COMP_PROP_DATA_LENGTH);
						script->properties[i][0] = '\0';

						/* map the buffer*/
						script->propertyMap.insert({ prop.first, script->properties[i] });

						i++;
					}
				}

			}
			ImGui::EndDragDropTarget();
		}

		/* retrieve script class */
		auto scriptClass = Application::Get().GetBehaviour().GetCustomClassByUID(script->script);

		if (scriptClass != nullptr)
		{
			for (auto& prop : scriptClass->Properties)
			{
				auto foundProperty = script->propertyMap.find(prop.first);
				if (foundProperty != script->propertyMap.end())
				{
					if (ScriptPropertyViewFactory::Create(prop.second, foundProperty->second) == false)
					{
						ImGui::TextColored({ 1.0f, 0.0f, 0.0f, 1.0f }, "Unabled to create view for property: %s", prop.first.c_str());
					}
				}
				else
				{
					int mapSize = script->propertyMap.size();
					auto newEntry = script->propertyMap.insert({ prop.first, &script->properties[mapSize] });
					if (ScriptPropertyViewFactory::Create(prop.second, newEntry.first->second) == false)
					{
						ImGui::TextColored({ 1.0f, 0.0f, 0.0f, 1.0f }, "Unabled to create view for property: %s", prop.first.c_str());
					}
				}
			}
		}
	}
}