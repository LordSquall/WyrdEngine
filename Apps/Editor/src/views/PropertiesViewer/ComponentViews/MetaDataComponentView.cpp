#pragma once

/* core wyrd includes */
#include <wyrdpch.h>
#include <core/Log.h>
#include <core/ecs/Components.h>

/* local includes */
#include "MetaDataComponentView.h"

#include "support/ImGuiUtils.h"

/* external includes */
#include <imgui.h>

namespace Wyrd::Editor
{
	COMPONENT_VIEW_FACTORY_REGISTER(MetaDataComponentView, "MetaData");

	void MetaDataComponentView::OnEditorRender(void* data)
	{
		/* Cast to the correct component */
		MetaDataComponent* metaData = (MetaDataComponent*)data;

		ImGui::Text("Name:");
		ImGui::SameLine();
		ImGui::InputText("##label", metaData->name, 32);
	}
}