#pragma once

/* core wyrd includes */
#include <wyrdpch.h>
#include <core/Log.h>

/* local includes */
#include "Vector2PropertyView.h"
#include "support/ImGuiUtils.h"

namespace Wyrd::Editor
{
	void Vector2PropertyView::OnEditorRender(const std::shared_ptr<ScriptProperty>& prop, void* value)
	{
		ImGui::InputFloat2(prop->GetName().c_str(), (float*)value);
	}

	SCRIPT_PROPERTY_VIEW_FACTORY_REGISTER(Vector2PropertyView);
}