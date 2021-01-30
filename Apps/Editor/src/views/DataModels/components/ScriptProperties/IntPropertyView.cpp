#pragma once

/* core osiris includes */
#include <osrpch.h>
#include <core/Log.h>

/* local includes */
#include "views/DataModels/components/ScriptProperties/IntPropertyView.h"
#include "support/ImGuiUtils.h"

namespace Osiris::Editor
{
	void IntPropertyView::DrawUI()
	{
		ImGui::DragInt(_Property->GetName().c_str(), _Property->GetValue());
	}

	SCRIPT_PROPERTY_VIEW_FACTORY_REGISTER(IntPropertyView);
}