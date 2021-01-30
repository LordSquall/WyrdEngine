#pragma once

/* core osiris includes */
#include <osrpch.h>
#include <core/Log.h>

/* local includes */
#include "views/DataModels/components/ScriptProperties/FloatPropertyView.h"
#include "support/ImGuiUtils.h"

namespace Osiris::Editor
{
	void FloatPropertyView::DrawUI()
	{
		ImGui::DragFloat(_Property->GetName().c_str(), _Property->GetValue());
	}

	SCRIPT_PROPERTY_VIEW_FACTORY_REGISTER(FloatPropertyView);
}