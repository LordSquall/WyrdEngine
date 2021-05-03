#pragma once

/* core wyrd includes */
#include <wyrdpch.h>
#include <core/Log.h>

/* local includes */
#include "views/DataModels/components/ScriptProperties/FloatPropertyView.h"
#include "support/ImGuiUtils.h"

namespace Wyrd::Editor
{
	void FloatPropertyView::DrawUI()
	{
		ImGui::DragFloat(_Property->GetName().c_str(), _Property->GetValue());
	}

	SCRIPT_PROPERTY_VIEW_FACTORY_REGISTER(FloatPropertyView);
}