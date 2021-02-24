#pragma once

/* core osiris includes */
#include <osrpch.h>
#include <core/Log.h>
#include <core/Structures.h>

/* local includes */
#include "views/DataModels/components/ScriptProperties/ColorPropertyView.h"
#include "support/ImGuiUtils.h"

namespace Osiris::Editor
{
	void ColorPropertyView::DrawUI()
	{
		ImGui::ColorEdit4(_Property->GetName().c_str(), (float*)_Property->GetValue());
	}

	SCRIPT_PROPERTY_VIEW_FACTORY_REGISTER(ColorPropertyView);
}