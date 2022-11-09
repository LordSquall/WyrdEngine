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
				if (ScriptPropertyViewFactory::Create(prop.second, nullptr) == false)
				{
					ImGui::TextColored({ 1.0f, 0.0f, 0.0f, 1.0f }, "Unabled to create view for property: %s", prop.first.c_str());
				}
			}
		}

		/* retrieve script class */
		//auto scriptClass = Application::Get().GetBehaviour().GetCustomClassByUID(script->scriptId);
		
		//if (scriptClass != nullptr)
		//{
		//	/* spin over all the properties in the class */
		//	for (auto& prop : scriptClass->Properties)
		//	{
		//		if (ScriptPropertyViewFactory::Create(prop.second, foundProperty->second.get()) == false)
		//		{
		//			ImGui::TextColored({ 1.0f, 0.0f, 0.0f, 1.0f }, "Unabled to create view for property: %s", prop.first.c_str());
		//		}
		//
		//		//auto foundProperty = script->propertiesData->find(prop.first);
		//		//if (foundProperty != script->propertiesData->end())
		//		//{
		//		//	if (ScriptPropertyViewFactory::Create(prop.second, foundProperty->second.get()) == false)
		//		//	{
		//		//		ImGui::TextColored({ 1.0f, 0.0f, 0.0f, 1.0f }, "Unabled to create view for property: %s", prop.first.c_str());
		//		//	}
		//		//}
		//		//else
		//		//{
		//		//	int mapSize = script->propertyMap.size();
		//		//	auto newEntry = script->propertyMap.insert({ prop.first, &script->properties[mapSize] });
		//		//	if (ScriptPropertyViewFactory::Create(prop.second, newEntry.first->second) == false)
		//		//	{
		//		//		ImGui::TextColored({ 1.0f, 0.0f, 0.0f, 1.0f }, "Unabled to create view for property: %s", prop.first.c_str());
		//		//	}
		//		//}
		//	}
		//	//for (auto& prop : scriptClass->Properties)
		//	//{
		//	//	if (ScriptPropertyViewFactory::Create(prop.second, nullptr) == false)
		//	//	{
		//	//		ImGui::TextColored({ 1.0f, 0.0f, 0.0f, 1.0f }, "Unabled to create view for property: %s", prop.first.c_str());
		//	//	}
		//	//
		//	//	//auto foundProperty = script->propertiesData->find(prop.first);
		//	//	//if (foundProperty != script->propertiesData->end())
		//	//	//{
		//	//	//	if (ScriptPropertyViewFactory::Create(prop.second, foundProperty->second) == false)
		//	//	//	{
		//	//	//		ImGui::TextColored({ 1.0f, 0.0f, 0.0f, 1.0f }, "Unabled to create view for property: %s", prop.first.c_str());
		//	//	//	}
		//	//	//}
		//	//	//else
		//	//	//{
		//	//	//	int mapSize = script->propertyMap.size();
		//	//	//	auto newEntry = script->propertyMap.insert({ prop.first, &script->properties[mapSize] });
		//	//	//	if (ScriptPropertyViewFactory::Create(prop.second, newEntry.first->second) == false)
		//	//	//	{
		//	//	//		ImGui::TextColored({ 1.0f, 0.0f, 0.0f, 1.0f }, "Unabled to create view for property: %s", prop.first.c_str());
		//	//	//	}
		//	//	//}
		//	//}
		//}
	}
}