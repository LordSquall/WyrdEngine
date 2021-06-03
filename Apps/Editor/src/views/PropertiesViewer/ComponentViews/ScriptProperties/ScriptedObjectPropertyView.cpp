#pragma once

/* core wyrd includes */
#include <wyrdpch.h>
#include <core/Log.h>
#include <core/Application.h>
#include <core/behaviour/Behaviour.h>
#include <core/scene/GameObject.h>

/* local includes */
#include "views/DataModels/components/ScriptProperties/ScriptedObjectPropertyView.h"
#include "services/ServiceManager.h"
#include "support/ImGuiUtils.h"

namespace Wyrd::Editor
{
	void ScriptedObjectPropertyView::DrawUI()
	{
		GameObject* gameObject = _Property->GetValue();

		ImGui::Text("Scripted Object: %s", gameObject != nullptr ? gameObject->name.c_str() : "<< Not Set >>");

		if (gameObject != nullptr)
		{
			if (ImGui::IsItemHovered())
			{
				ImGui::BeginTooltip();
				ImGui::Text("UID: %s", gameObject->uid.str().c_str());
				ImGui::EndTooltip();
			}
		}

		if (ImGui::BeginDragDropTarget())
		{
			if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("GAMEOBJECT_PAYLOAD"))
			{
				UID* gameobjectUID = (UID*)payload->Data;
				GameObject* foundGameObject = ServiceManager::Get<WorkspaceService>(ServiceManager::Workspace)->GetLoadedScene()->FindGameObject(*gameobjectUID);
				if (foundGameObject != nullptr)
				{
					ScriptComponent* component = foundGameObject->FindScriptComponent("SecondaryClass");
					if (component != nullptr)
					{
						_Property->SetValue(foundGameObject);
					}
				}
				else
				{
					WYRD_TRACE("Unable to find gameobject UID: {0}", gameobjectUID->str());
				}

			}
			ImGui::EndDragDropTarget();
		}
	}

	SCRIPT_PROPERTY_VIEW_FACTORY_REGISTER(ScriptedObjectPropertyView);
}