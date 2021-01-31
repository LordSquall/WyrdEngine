#pragma once

/* core osiris includes */
#include <osrpch.h>
#include <core/Log.h>
#include <core/Application.h>
#include <core/behaviour/Behaviour.h>
#include <core/scene/GameObject.h>

/* local includes */
#include "views/DataModels/components/ScriptProperties/GameObjectPropertyView.h"
#include "services/ServiceManager.h"
#include "support/ImGuiUtils.h"

namespace Osiris::Editor
{
	void GameObjectPropertyView::DrawUI()
	{
		GameObject* gameObject = _Property->GetValue();

		ImGui::Text(gameObject != nullptr ? gameObject->name.c_str() : "<< Not Set >>");

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
			if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("DND_GAMEOBJECT"))
			{
				std::shared_ptr<GameObject>* gameObject = (std::shared_ptr<GameObject>*)payload->Data;
				OSR_TRACE("UID: {0}", (*gameObject)->uid.str());

				_Property->SetValue(gameObject->get());
			}
			ImGui::EndDragDropTarget();
		}
	}

	SCRIPT_PROPERTY_VIEW_FACTORY_REGISTER(GameObjectPropertyView);
}