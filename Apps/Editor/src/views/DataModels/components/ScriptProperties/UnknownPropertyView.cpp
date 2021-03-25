#pragma once

/* core osiris includes */
#include <osrpch.h>
#include <core/Log.h>

/* local includes */
#include "views/DataModels/components/ScriptProperties/UnknownPropertyView.h"
#include "support/ImGuiUtils.h"

namespace Osiris::Editor
{
	void UnknownPropertyView::DrawUI()
	{
		ImGui::TextColored(ImVec4(1.0f, 0.0f, 0.0f, 1.0f), "%s : Unknown Variable Type", _name.c_str());
	}

	SCRIPT_PROPERTY_VIEW_FACTORY_REGISTER(UnknownPropertyView);
}