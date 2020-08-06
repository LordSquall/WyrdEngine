#pragma once
#include "osrpch.h"

#include <imgui.h>

#include "PhysicsComponentView.h"

namespace Osiris::Editor
{
	void PhysicsComponentView::OnPropertyEditorDraw()
	{
		if (ImGui::TreeNodeEx("Phsyics", ImGuiTreeNodeFlags_DefaultOpen))
		{
			ImGui::Text("This is Physics Component View");
			///ImGui::Text((_BaseComponent != nullptr) ? _BaseComponent->GetName().c_str() : "<<unbound>>");
			/*if (ImGui::BeginDragDropTarget())
			{
				if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("ASSET_DND_PAYLOAD"))
				{
					uint32_t payload_n = *(const uint32_t*)payload->Data;
					OwnerGameObject->script.SetScriptResource(ServiceManager::Get<ResourceService>(ServiceManager::Resources)->GetScriptByUID(payload_n));
				}
				ImGui::EndDragDropTarget();
			}
			ImGui::SameLine();
			if (ImGui::IconButton(_RemoveIcon, 1, ImVec2(16.0f, 16.0f), _ScriptResource != nullptr) == true)
			{
				_ScriptResource = nullptr;
			}*/

			ImGui::TreePop();
		}
	}
}