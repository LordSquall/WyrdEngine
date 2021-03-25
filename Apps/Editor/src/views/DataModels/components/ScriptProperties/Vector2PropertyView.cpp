#pragma once

/* core osiris includes */
#include <osrpch.h>
#include <core/Log.h>

/* local includes */
#include "views/DataModels/components/ScriptProperties/Vector2PropertyView.h"
#include "support/ImGuiUtils.h"

namespace Osiris::Editor
{
	void Vector2PropertyView::DrawUI()
	{
		ImGui::InputFloat2(_Property->GetName().c_str(), (float*)_Property->GetValue());
	}

	SCRIPT_PROPERTY_VIEW_FACTORY_REGISTER(Vector2PropertyView);
}