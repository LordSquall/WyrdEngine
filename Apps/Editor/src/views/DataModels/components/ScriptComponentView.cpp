#pragma once

/* core osiris includes */
#include <osrpch.h>
#include <core/behaviour/MonoUtils.h>

/* local includes */
#include "ScriptComponentView.h"
#include "services/ServiceManager.h"
#include "support/ImGuiUtils.h"
#include "views/DataModels/components/ScriptProperties/UnknownPropertyView.h"

/* external includes */
#include <imgui.h>
#include <core/Log.h>

namespace Osiris::Editor
{
	ScriptComponentView::ScriptComponentView(ScriptComponent* baseComponent) : _BaseComponent(baseComponent)
	{
		RebuildPropertyViews();
	}

	void ScriptComponentView::OnPropertyEditorDraw()
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
			if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("SCRIPT_ASSET_PAYLOAD"))
			{
				std::shared_ptr<ScriptRes> scriptRes = *(std::shared_ptr<ScriptRes>*)payload->Data;
				_BaseComponent->SetClass(scriptRes->Script);
				_BaseComponent->SetUUID(scriptRes->GetResourceID());
				RebuildPropertyViews();
			}
			ImGui::EndDragDropTarget();
		}

		for (auto& prop : _PropertyViews)
		{
			ImGui::PushID(&prop);
			prop->DrawUI();
			ImGui::PopID();
		}
	}

	void ScriptComponentView::RebuildPropertyViews()
	{
		_PropertyViews.clear();

		if (_BaseComponent->Properties != nullptr)
		{
			for (auto& prop : *_BaseComponent->Properties)
			{
				std::unique_ptr<ScriptPropertyView> view = std::move(ScriptPropertyViewFactory::Create(prop));
				if (view != nullptr)
				{
					_PropertyViews.push_back(std::move(view));
				}
				else
				{
					_PropertyViews.push_back(std::make_unique<UnknownPropertyView>(prop));
				}
			}
		}
	}
}