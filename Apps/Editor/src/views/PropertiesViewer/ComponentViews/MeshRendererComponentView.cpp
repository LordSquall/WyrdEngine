/* core wyrd includes */
#include <wyrdpch.h>
#include <core/Log.h>
#include <core/ecs/Components.h>
#include <core/Application.h>

/* local includes */
#include "MeshRendererComponentView.h"

#include "support/ImGuiUtils.h"

/* external includes */
#include <imgui.h>

namespace Wyrd::Editor
{
	COMPONENT_VIEW_FACTORY_REGISTER(MeshRendererComponentView, "MeshRenderer");

	void MeshRendererComponentView::OnEditorRender(Entity e, void* data)
	{
		/* Cast to the correct component */
		MeshRendererComponent* meshRenderer = (MeshRendererComponent*)data;
		
		ImGui::ColorPicker4("Pos ", (float*)&meshRenderer->color);

		ImGui::Text(meshRenderer->model.str().c_str());
		if (ImGui::BeginDragDropTarget())
		{
			if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload(IMGUI_DND_MODEL))
			{
				UID* modelUID = (UID*)payload->Data;
				meshRenderer->model = *modelUID;
			}
			ImGui::EndDragDropTarget();
		}

		ImGui::Text(meshRenderer->material.str().c_str());
		if (ImGui::BeginDragDropTarget())
		{
			if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload(IMGUI_DND_MATERIAL))
			{
				UID* materialUID = (UID*)payload->Data;
				meshRenderer->material = *materialUID;
			}
			ImGui::EndDragDropTarget();
		}
	}
}