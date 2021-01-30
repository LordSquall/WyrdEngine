#pragma once

/* core osiris includes */
#include <osrpch.h>
#include <core/Log.h>

/* local includes */
#include "views/DataModels/components/ScriptProperties/BoolPropertyView.h"
#include "support/ImGuiUtils.h"

namespace Osiris::Editor
{
	void BoolPropertyView::DrawUI()
	{
		ImGui::Checkbox(_Property->GetName().c_str(), _Property->GetValue());
	}

	SCRIPT_PROPERTY_VIEW_FACTORY_REGISTER(BoolPropertyView);
}