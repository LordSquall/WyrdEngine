#pragma once

/* core osiris includes */
#include <osrpch.h>
#include <core/Log.h>

/* local includes */
#include "views/DataModels/components/ScriptProperties/StringPropertyView.h"
#include "support/ImGuiUtils.h"

namespace Osiris::Editor
{
	void StringPropertyView::DrawUI()
	{
		ImGui::InputText(_Property->GetName().c_str(), _Property->GetValue());
	}

	SCRIPT_PROPERTY_VIEW_FACTORY_REGISTER(StringPropertyView);
}