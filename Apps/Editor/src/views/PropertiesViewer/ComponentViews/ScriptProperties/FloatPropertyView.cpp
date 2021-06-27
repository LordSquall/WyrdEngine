#pragma once

/* core wyrd includes */
#include <wyrdpch.h>
#include <core/Log.h>

/* local includes */
#include "FloatPropertyView.h"
#include "support/ImGuiUtils.h"

namespace Wyrd::Editor
{
	void FloatPropertyView::OnEditorRender(const std::shared_ptr<ScriptProperty>& prop, void* value)
	{
		ImGui::DragFloat(prop->GetName().c_str(), (float*)value);
	}

	SCRIPT_PROPERTY_VIEW_FACTORY_REGISTER(FloatPropertyView);
}