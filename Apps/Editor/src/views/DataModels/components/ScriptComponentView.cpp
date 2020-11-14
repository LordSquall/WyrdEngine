#pragma once

/* core osiris includes */
#include <osrpch.h>
#include <core/behaviour/MonoUtils.h>

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
			if (_BaseComponent->GetClass() != nullptr)
			{
				ImGui::Text(_BaseComponent->GetClass()->GetName().c_str());
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
					_BaseComponent->SetClass(ServiceManager::Get<ResourceService>(ServiceManager::Resources)->GetScriptByUID(payload_n)->Script);
				}
				ImGui::EndDragDropTarget();
			}

			for (auto& prop : _BaseComponent->Properties)
			{
				switch (prop.type)
				{
				case ScriptedClass::PropType::INT:
					DrawIntUI(prop);
					break;
				case ScriptedClass::PropType::FLOAT:
					DrawFloatUI(prop);
					break;
				default:
					ImGui::Text("%s - Unknown Type", prop.name.c_str());
				}
			}

			ImGui::TreePop();
		}
	}

	void ScriptComponentView::DrawIntUI(ScriptedClass::PropertyDesc& prop)
	{
		int value = prop.value.i;
		if (ImGui::DragInt(prop.name.c_str(), &value))
		{
			prop.value.i = value;
		}
	}

	void ScriptComponentView::DrawFloatUI(ScriptedClass::PropertyDesc& prop)
	{
		float value = prop.value.f;
		if (ImGui::DragFloat(prop.name.c_str(), &value))
		{
			prop.value.f = value;
		}
	}
}