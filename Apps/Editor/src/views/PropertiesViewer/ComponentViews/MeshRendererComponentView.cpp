/* core wyrd includes */
#include <wyrdpch.h>
#include <core/Log.h>
#include <core/ecs/Components.h>
#include <core/Application.h>

/* local includes */
#include "MeshRendererComponentView.h"
#include "services/ServiceManager.h"

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

		std::shared_ptr<ResourceService> resources = ServiceManager::Get<ResourceService>();

		ModelResRef modelResRef = resources->GetResourceByID<ModelRes>(meshRenderer->model);

		if (modelResRef != nullptr)
		{
			ImGui::Text(modelResRef->GetName().c_str());
		}
		else
		{
			ImGui::Text(meshRenderer->model.str().c_str());
		}
		if (ImGui::BeginDragDropTarget())
		{
			if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload(IMGUI_DND_MODEL))
			{
				UID* modelUID = (UID*)payload->Data;
				meshRenderer->model = *modelUID;
			}
			ImGui::EndDragDropTarget();
		}
	}
}