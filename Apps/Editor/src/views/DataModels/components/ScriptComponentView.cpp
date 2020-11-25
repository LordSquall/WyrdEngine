#pragma once

/* core osiris includes */
#include <osrpch.h>
#include <core/behaviour/MonoUtils.h>

/* local includes */
#include "ScriptComponentView.h"
#include "services/ServiceManager.h"
#include "support/ImGuiUtils.h"

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
				case ScriptedClass::PropType::STRING:
					DrawStringUI(prop);
					break;
				case ScriptedClass::PropType::OBJECT:
					DrawObjectUI(prop);
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
		int value = prop.intVal;
		if (ImGui::DragInt(prop.name.c_str(), &value))
		{
			prop.intVal = value;
		}
	}

	void ScriptComponentView::DrawFloatUI(ScriptedClass::PropertyDesc& prop)
	{
		float value = prop.floatVal;
		if (ImGui::DragFloat(prop.name.c_str(), &value))
		{
			prop.floatVal = value;
		}
	}

	void ScriptComponentView::DrawStringUI(ScriptedClass::PropertyDesc& prop)
	{
		ImGui::InputText(prop.name.c_str(), &prop.stringVal);
	}

	void ScriptComponentView::DrawObjectUI(ScriptedClass::PropertyDesc& prop)
	{
		ImGui::InputText(prop.name.c_str(), &prop.objectVal);
	}
}