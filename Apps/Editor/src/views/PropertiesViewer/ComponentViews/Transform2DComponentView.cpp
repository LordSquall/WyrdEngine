#pragma once

/* core wyrd includes */
#include <wyrdpch.h>
#include <core/Log.h>
#include <core/ecs/Components.h>

/* local includes */
#include "Transform2DComponentView.h"

#include "support/ImGuiUtils.h"

/* external includes */
#include <imgui.h>

namespace Wyrd::Editor
{
	COMPONENT_VIEW_FACTORY_REGISTER(Tranform2DComponentView, "Transform2D");

	void Tranform2DComponentView::OnEditorRender(Entity e, void* data)
	{
		/* Cast to the correct component */
		Transform2DComponent* transform2D = (Transform2DComponent*)data;

		ImGui::InputFloat2("Pos     ", (float*)&transform2D->position);
		ImGui::InputFloat2("Rotation Origin     ", (float*)&transform2D->rotationOrigin);
		ImGui::InputFloat("Angle     ", (float*)&transform2D->rotation);
		ImGui::SliderFloat("Angle     ", (float*)&transform2D->rotation, 0.0f, 360.0f);
	}
}