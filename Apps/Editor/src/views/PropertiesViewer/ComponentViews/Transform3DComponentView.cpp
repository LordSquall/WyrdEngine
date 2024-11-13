/* core wyrd includes */
#include <wyrdpch.h>
#include <core/Log.h>
#include <core/ecs/Components.h>
#include <core/Application.h>

/* local includes */
#include "Transform3DComponentView.h"
#include "services/ServiceManager.h"

#include "support/ImGuiUtils.h"

/* external includes */
#include <imgui.h>

namespace Wyrd::Editor
{
	COMPONENT_VIEW_FACTORY_REGISTER(Tranform3DComponentView, "Transform3D");

	void Tranform3DComponentView::OnEditorRender(Entity e, void* data)
	{
		/* Cast to the correct component */
		Transform3DComponent* transform3D = (Transform3DComponent*)data;
		
		ImGui::InputFloat3("Pos     ", (float*)&transform3D->position);
		ImGui::InputFloat3("Rot     ", (float*)&transform3D->rotation);
		ImGui::InputFloat3("Scale     ", (float*)&transform3D->scale);
	}
}