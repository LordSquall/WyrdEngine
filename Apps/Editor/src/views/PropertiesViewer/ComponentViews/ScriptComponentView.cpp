#pragma once

/* core wyrd includes */
#include <wyrdpch.h>
#include <core/Log.h>
#include <core/ecs/Components.h>

/* local includes */
#include "ScriptComponentView.h"

#include "support/ImGuiUtils.h"

/* external includes */
#include <imgui.h>

namespace Wyrd::Editor
{
	COMPONENT_VIEW_FACTORY_REGISTER(ScriptComponentView, "Script");

	void ScriptComponentView::OnEditorRender(void* data)
	{
		/* Cast to the correct component */
		ScriptComponent* script = (ScriptComponent*)data;

		ImGui::Text("Script: %s", script->script.str().c_str());
		if (ImGui::BeginDragDropTarget())
		{
			if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("DND_SCRIPT"))
			{
				UID* scriptUID = (UID*)payload->Data;
				script->script = *scriptUID;
			}
			ImGui::EndDragDropTarget();
		}
	}
}