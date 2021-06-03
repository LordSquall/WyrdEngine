#pragma once

/* core wyrd includes */
#include <wyrdpch.h>
#include <core/Log.h>

/* local includes */
#include "views/DataModels/components/ScriptProperties/Vector3PropertyView.h"
#include "support/ImGuiUtils.h"

namespace Wyrd::Editor
{
	void Vector3PropertyView::DrawUI()
	{
		ImGui::InputFloat3(_Property->GetName().c_str(), (float*)_Property->GetValue());
	}

	SCRIPT_PROPERTY_VIEW_FACTORY_REGISTER(Vector3PropertyView);
}