#pragma once

/* core osiris includes */
#include <osrpch.h>

/* local includes */
#include "ScriptComponentView.h"
#include "services/ServiceManager.h"

/* external includes */
#include <imgui.h>

namespace Osiris::Editor
{
	void ScriptComponentView::OnPropertyEditorDraw()
	{
		if (ImGui::TreeNodeEx("Script", ImGuiTreeNodeFlags_DefaultOpen))
		{
			if (_BaseComponent->Class != nullptr)
			{
				ImGui::Text(_BaseComponent->Class->GetName().c_str());
			}
			else
			{
				ImGui::Text("<<unbound>>");
			}

			if (ImGui::BeginDragDropTarget())
			{
				if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("ASSET_DND_PAYLOAD"))
				{
					int32_t payload_n = *(const uint32_t*)payload->Data;
					_BaseComponent->Class = ServiceManager::Get<ResourceService>(ServiceManager::Resources)->GetScriptByUID(payload_n)->Script;
				}
				ImGui::EndDragDropTarget();
			}

			ImGui::TreePop();
		}
	}
}