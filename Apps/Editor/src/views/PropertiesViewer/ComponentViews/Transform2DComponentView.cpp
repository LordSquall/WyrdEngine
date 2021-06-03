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

	void Tranform2DComponentView::OnEditorRender(void* data)
	{
		/* Cast to the correct component */
		ECSTransform2DComponent* transform2D = (ECSTransform2DComponent*)data;

		ImGui::InputFloat2("Pos     ", (float*)&transform2D->position);
	}
}